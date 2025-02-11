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

#include "tsembd.hpp"

static void timestep_embedding_f32(
        const float * timesteps, float * dst, const int nb1,
        const int dim, const int max_period, const sycl::nd_item<3> &item_ct1) {
    // item_ct1.get_group(1)(blockIDx.y): idx of timesteps->ne[0]
    // item_ct1.get_group(2) (blockIDx.x): idx of ((dim + 1) / 2) / BLOCK_SIZE
    int i = item_ct1.get_group(1);
    int j = item_ct1.get_local_id(2) + item_ct1.get_group(2) * item_ct1.get_local_range(2);
    float * embed_data = (float *)((char *)dst +  i*nb1);

    if (dim % 2 != 0 && j == ((dim + 1) / 2)) {
        embed_data[dim] = 0.f;
    }

    int half = dim / 2;
    if (j >= half) {
        return;
    }

    float timestep = timesteps[i];
    float freq = (float)sycl::native::exp(-(sycl::log((float)max_period)) * j / half);
    float arg = timestep * freq;
    embed_data[j] = sycl::cos(arg);
    embed_data[j + half] = sycl::sin(arg);
}

static void timestep_embedding_f32_sycl(
        const float * x, float * dst, const int ne00, const int nb1,
        const int dim, const int max_period, const queue_ptr& stream) {
    // As the kernel returns when thread.idx is larger than dim/2, the half_ceil does not need to pad
    int half_ceil = dim / 2;
    int num_blocks = (half_ceil + SYCL_TIMESTEP_EMBEDDING_BLOCK_SIZE - 1) / SYCL_TIMESTEP_EMBEDDING_BLOCK_SIZE;
    sycl::range<3> block_dims(1, 1, SYCL_TIMESTEP_EMBEDDING_BLOCK_SIZE);
    sycl::range<3> gridDim(1, ne00, num_blocks);
    stream->parallel_for(
        sycl::nd_range<3>(
            gridDim * block_dims, block_dims),
        [=](sycl::nd_item<3> item_ct1) {
            timestep_embedding_f32(
                x, dst, nb1, dim, max_period, item_ct1
            );
        });
}

void ggml_sycl_op_timestep_embedding(ggml_backend_sycl_context & ctx, ggml_tensor * dst) {
    const ggml_tensor *src0 = dst->src[0];
    const ggml_tensor *src1 = dst->src[1];
    const float * src0_d = (const float *)src0->data;
    float * dst_d = (float *)dst->data;
    dpct::queue_ptr stream = ctx.stream();

    GGML_ASSERT(src0->type == GGML_TYPE_F32);
    GGML_ASSERT(dst->type == GGML_TYPE_F32);

    const int dim = dst->op_params[0];
    const int max_period = dst->op_params[1];

    timestep_embedding_f32_sycl(src0_d, dst_d, src0->ne[0], dst->nb[1], dim, max_period, stream);
    GGML_UNUSED(src1);
}
