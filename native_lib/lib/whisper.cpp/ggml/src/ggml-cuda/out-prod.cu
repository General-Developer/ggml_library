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
#include "out-prod.cuh"

#include <cstdint>

void ggml_cuda_out_prod(ggml_backend_cuda_context & ctx, ggml_tensor * dst) {
    const ggml_tensor * src0 = dst->src[0];
    const ggml_tensor * src1 = dst->src[1];

    GGML_TENSOR_BINARY_OP_LOCALS

    GGML_ASSERT(src0->type == GGML_TYPE_F32);
    GGML_ASSERT(src1->type == GGML_TYPE_F32);
    GGML_ASSERT(dst->type  == GGML_TYPE_F32);

    GGML_ASSERT(ne01 == ne11);
    GGML_ASSERT(ne0 == ne00);
    GGML_ASSERT(ne1 == ne10);

    GGML_ASSERT(ne2 % src0->ne[2] == 0);
    GGML_ASSERT(ne3 % src0->ne[3] == 0);

    GGML_ASSERT(ne2 == src1->ne[2]);
    GGML_ASSERT(ne3 == src1->ne[3]);

    const float * src0_d = (const float *) src0->data;
    const float * src1_d = (const float *) src1->data;
    float       *  dst_d = (float       *)  dst->data;

    cudaStream_t   stream = ctx.stream();
    cublasHandle_t handle = ctx.cublas_handle();

    const float alpha = 1.0f;
    const float beta = 0.0f;

    CUBLAS_CHECK(cublasSetStream(handle, stream));

    const int64_t lda = nb01 / sizeof(float);
    const int64_t ldc = nb1  / sizeof(float);

    const bool src1_T = ggml_is_transposed(src1);
    const cublasOperation_t src1_cublas_op =  src1_T ? CUBLAS_OP_N : CUBLAS_OP_T;
    const int64_t           ldb            = (src1_T ?        nb10 :        nb11) /  sizeof(float);
    GGML_ASSERT(                             (src1_T ?        nb11 :        nb10) == sizeof(float));

    // data strides in dimensions 2/3
    const size_t s02 = nb02 / sizeof(float);
    const size_t s03 = nb03 / sizeof(float);
    const size_t s12 = nb12 / sizeof(float);
    const size_t s13 = nb13 / sizeof(float);
    const size_t s2  = nb2  / sizeof(float);
    const size_t s3  = nb3  / sizeof(float);

    // dps == dst per src0, used for group query attention
    const int64_t dps2 = ne2 / ne02;
    const int64_t dps3 = ne3 / ne03;

    // TODO batched matrix multiplication
    for (int64_t i3 = 0; i3 < ne3; ++i3) {
        for (int64_t i2 = 0; i2 < ne2; ++i2) {
            CUBLAS_CHECK(
                cublasSgemm(handle, CUBLAS_OP_N, src1_cublas_op,
                        ne0, ne1, ne01,
                        &alpha, src0_d + (i3/dps3)*s03 + (i2/dps2)*s02, lda,
                                src1_d +  i3      *s13 +  i2      *s12, ldb,
                        &beta,  dst_d  +  i3      *s3  +  i2      *s2,  ldc));
        }
    }
}
