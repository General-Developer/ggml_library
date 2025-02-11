/* <!-- START LICENSE -->


This Software / Program / Source Code Created By Developer From Company GLOBAL CORPORATION
Social Media:

   - Youtube: https://youtube.com/@Global_Corporation 
   - Github: https://github.com/globalcorporation
   - TELEGRAM: https://t.me/GLOBAL_CORP_ORG_BOT

All code script in here created 100% original without copy / steal from other code if we copy we add description source at from top code

If you wan't edit you must add credit me (don't change)

If this Software / Program / Source Code has you

Jika Program ini milik anda dari hasil beli jasa developer di (Global Corporation / apapun itu dari turunan itu jika ada kesalahan / bug / ingin update segera lapor ke sub)

Misal anda beli Beli source code di Slebew CORPORATION anda lapor dahulu di slebew jangan lapor di GLOBAL CORPORATION!

Jika ada kendala program ini (Pastikan sebelum deal project tidak ada negosiasi harga)
Karena jika ada negosiasi harga kemungkinan

1. Software Ada yang di kurangin
2. Informasi tidak lengkap
3. Bantuan Tidak Bisa remote / full time (Ada jeda)

Sebelum program ini sampai ke pembeli developer kami sudah melakukan testing

jadi sebelum nego kami sudah melakukan berbagai konsekuensi jika nego tidak sesuai ? 
Bukan maksud kami menipu itu karena harga yang sudah di kalkulasi + bantuan tiba tiba di potong akhirnya bantuan / software kadang tidak lengkap


<!-- END LICENSE --> */
//
// MIT license
// Copyright (C) 2024 Intel Corporation
// SPDX-License-Identifier: MIT
//

//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//

#include "common.hpp"

#include "ggml-backend-impl.h"
#include "ggml-impl.h"

int get_current_device_id() {
  return dpct::dev_mgr::instance().current_device_id();
}

void* ggml_sycl_host_malloc(size_t size) try {
  if (getenv("GGML_SYCL_NO_PINNED") != nullptr) {
    return nullptr;
  }

  void* ptr = nullptr;
  // allow to use dpct::get_in_order_queue() for host malloc
  dpct::err0 err = CHECK_TRY_ERROR(
      ptr = (void*)sycl::malloc_host(size, dpct::get_in_order_queue()));

  if (err != 0) {
    // clear the error
    GGML_LOG_ERROR("WARNING: failed to allocate %.2f MB of pinned memory: %s\n", size / 1024.0 / 1024.0,    "syclGetErrorString is not supported");
    return nullptr;
  }

  return ptr;
} catch (sycl::exception const& exc) {
  std::cerr << exc.what() << "Exception caught at file:" << __FILE__
            << ", line:" << __LINE__ << std::endl;
  std::exit(1);
}

void ggml_sycl_host_free(void* ptr) try {
  // allow to use dpct::get_in_order_queue() for host malloc
  SYCL_CHECK(CHECK_TRY_ERROR(sycl::free(ptr, dpct::get_in_order_queue())));
} catch (sycl::exception const& exc) {
  std::cerr << exc.what() << "Exception caught at file:" << __FILE__
            << ", line:" << __LINE__ << std::endl;
  std::exit(1);
}

bool gpu_has_xmx(sycl::device &dev) {
    return dev.has(sycl::aspect::ext_intel_matrix);
}

int64_t downsample_sycl_global_range(int64_t accumulate_block_num, int64_t block_size) {
  const int64_t max_range = std::numeric_limits<int>::max();
  int64_t sycl_down_blk_size = block_size;
  int64_t global_range = accumulate_block_num * sycl_down_blk_size;
  while(global_range > max_range) {
      sycl_down_blk_size /= 2;
      global_range = accumulate_block_num * sycl_down_blk_size;
  }
  return sycl_down_blk_size;
}

void ggml_sycl_op_flatten(ggml_backend_sycl_context & ctx, const ggml_tensor *src0,
                                 const ggml_tensor *src1, ggml_tensor *dst,
                                 const ggml_sycl_op_flatten_t op) try {

    const bool use_src1 = src1 != nullptr;
    if(use_src1)
      GGML_ASSERT(strcmp(src1->buffer->buft->iface.get_name(src1->buffer->buft), GGML_SYCL_NAME "_Split") != 0);
    GGML_ASSERT(strcmp(dst->buffer->buft->iface.get_name(dst->buffer->buft), GGML_SYCL_NAME "_Split") != 0);

    // dd = data device
    float * src0_ddf = (float *) src0->data;
    float * src1_ddf = use_src1 ? (float *) src1->data : nullptr;
    float *  dst_ddf = (float *) dst->data;

    ggml_sycl_pool_alloc<float> src0_f(ctx.pool());
    ggml_sycl_pool_alloc<float> src1_f(ctx.pool());
    ggml_sycl_pool_alloc<float>  dst_f(ctx.pool());

    ggml_sycl_set_device(ctx.device);
    queue_ptr main_stream = ctx.stream();
    // GGML_SYCL_DEBUG("ctx.device=%d, main_stream=%p src0_on_device=%d, src1_on_device=%d, dst_on_device=%d\n",
        // ctx.device, main_stream, src0_on_device, src1_on_device, dst_on_device);

    // do the computation
    op(ctx, src0, src1, dst, src0_ddf, src1_ddf, dst_ddf, main_stream);
    // print_ggml_tensor("tensor", dst);
}
catch (sycl::exception const &exc) {

  std::cerr << exc.what() << "Exception caught at file:" << __FILE__
            << ", line:" << __LINE__ << std::endl;
  std::exit(1);
}
