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
#include <sycl/sycl.hpp>
#include <oneapi/mkl.hpp>
#include "outprod.hpp"


void ggml_sycl_op_out_prod(ggml_backend_sycl_context& ctx, ggml_tensor* dst) {
    const ggml_tensor *src0 = dst->src[0];
    const ggml_tensor *src1 = dst->src[1];

    GGML_ASSERT(src0->type == GGML_TYPE_F32);
    GGML_ASSERT(src1->type == GGML_TYPE_F32);
    GGML_ASSERT(dst->type == GGML_TYPE_F32);
    GGML_ASSERT(ggml_is_contiguous(src0));
    GGML_ASSERT(ggml_is_contiguous(dst));

    GGML_TENSOR_BINARY_OP_LOCALS

    // Get SYCL queue
    dpct::queue_ptr stream = ctx.stream();

    // Dimension checks
    GGML_ASSERT(ne01 == ne11);  // Inner dimensions must match
    GGML_ASSERT(ne0 == ne00);   // Output rows match src0 rows
    GGML_ASSERT(ne1 == ne10);   // Output cols match src1 cols

    // Get data pointers
    const float* src0_d = (const float*)src0->data;
    const float* src1_d = (const float*)src1->data;
    float* dst_d = (float*)dst->data;

    // GEMM parameters
    const float alpha = 1.0f;
    const float beta = 0.0f;

    // Handle transposition of src1
    const bool src1_T = ggml_is_transposed(src1);
    const oneapi::mkl::transpose src1_op =
        src1_T ? oneapi::mkl::transpose::nontrans : oneapi::mkl::transpose::trans;
    const int64_t ldb = (src1_T ? nb10 : nb11) / sizeof(float);

    try {
        // Perform matrix multiplication using oneMKL GEMM
#ifdef GGML_SYCL_NVIDIA
        oneapi::mkl::blas::column_major::gemm(oneapi::mkl::backend_selector<oneapi::mkl::backend::cublas>{ *stream },
                                              oneapi::mkl::transpose::nontrans, src1_op, ne0, ne1, ne01, alpha, src0_d,
                                              ne00, src1_d, ldb, beta, dst_d, ne0);
#else
        oneapi::mkl::blas::column_major::gemm(*stream, oneapi::mkl::transpose::nontrans, src1_op, ne0, ne1, ne01, alpha,
                                              src0_d, ne00, src1_d, ldb, beta, dst_d, ne0);
#endif
    }
    catch (sycl::exception const& exc) {
        std::cerr << exc.what() << std::endl;
        GGML_ASSERT(false);
    }
}
