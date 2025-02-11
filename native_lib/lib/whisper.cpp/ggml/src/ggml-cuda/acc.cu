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
#include "acc.cuh"

static __global__ void acc_f32(const float * x, const float * y, float * dst, const int ne,
    const int ne10, const int ne11, const int ne12,
    const int nb1, const int nb2, int offset) {
    const int i = blockDim.x * blockIdx.x + threadIdx.x;
    if (i >= ne) {
        return;
    }
    int src1_idx = i - offset;
    int oz = src1_idx / nb2;
    int oy = (src1_idx - (oz * nb2)) / nb1;
    int ox = src1_idx % nb1;
    if (src1_idx >= 0 && ox < ne10 && oy < ne11 && oz < ne12) {
        dst[i] = x[i] + y[ox + oy * ne10 + oz * ne10 * ne11];
    } else {
        dst[i] = x[i];
    }
}

static void acc_f32_cuda(const float * x, const float * y, float * dst, const int n_elements,
    const int ne10, const int ne11, const int ne12,
    const int nb1, const int nb2, const int offset, cudaStream_t stream) {
    int num_blocks = (n_elements + CUDA_ACC_BLOCK_SIZE - 1) / CUDA_ACC_BLOCK_SIZE;
    acc_f32<<<num_blocks, CUDA_ACC_BLOCK_SIZE, 0, stream>>>(x, y, dst, n_elements, ne10, ne11, ne12, nb1, nb2, offset);
}

void ggml_cuda_op_acc(ggml_backend_cuda_context & ctx, ggml_tensor * dst) {
    const ggml_tensor * src0 = dst->src[0];
    const ggml_tensor * src1 = dst->src[1];
    const float * src0_d = (const float *)src0->data;
    const float * src1_d = (const float *)src1->data;
    float * dst_d = (float *)dst->data;
    cudaStream_t stream = ctx.stream();

    GGML_ASSERT(src0->type == GGML_TYPE_F32);
    GGML_ASSERT(src1->type == GGML_TYPE_F32);
    GGML_ASSERT( dst->type == GGML_TYPE_F32);
    GGML_ASSERT(dst->ne[3] == 1); // just 3D tensors supported

    int nb1 = dst->op_params[0] / 4; // 4 bytes of float32
    int nb2 = dst->op_params[1] / 4; // 4 bytes of float32
    // int nb3 = dst->op_params[2] / 4; // 4 bytes of float32 - unused
    int offset = dst->op_params[3] / 4; // offset in bytes

    acc_f32_cuda(src0_d, src1_d, dst_d, ggml_nelements(dst), src1->ne[0], src1->ne[1], src1->ne[2], nb1, nb2, offset, stream);
}
