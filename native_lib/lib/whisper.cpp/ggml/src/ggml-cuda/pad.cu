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
#include "pad.cuh"

static __global__ void pad_f32(const float * x, float * dst, const int ne0, const int ne00, const int ne01, const int ne02, const int ne03) {
    // blockIdx.z: idx of ne2*ne3, aka ne02*ne03
    // blockIdx.y: idx of ne1
    // blockIDx.x: idx of ne0 / BLOCK_SIZE
    int nidx = threadIdx.x + blockIdx.x * blockDim.x;
    if (nidx >= ne0) {
        return;
    }

    // operation
    int offset_dst =
        nidx +
        blockIdx.y * ne0 +
        blockIdx.z * ne0 * gridDim.y;
    if (nidx < ne00 && blockIdx.y < ne01 && blockIdx.z < ne02*ne03) {
        int offset_src =
            nidx +
            blockIdx.y * ne00 +
            blockIdx.z * ne00 * ne01;
        dst[offset_dst] = x[offset_src];
    } else {
        dst[offset_dst] = 0.0f;
    }
}

static void pad_f32_cuda(const float * x, float * dst,
    const int ne00, const int ne01, const int ne02, const int ne03,
    const int ne0, const int ne1, const int ne2, const int ne3, cudaStream_t stream) {
    int num_blocks = (ne0 + CUDA_PAD_BLOCK_SIZE - 1) / CUDA_PAD_BLOCK_SIZE;
    dim3 gridDim(num_blocks, ne1, ne2*ne3);
    pad_f32<<<gridDim, CUDA_PAD_BLOCK_SIZE, 0, stream>>>(x, dst, ne0, ne00, ne01, ne02, ne03);
}

void ggml_cuda_op_pad(ggml_backend_cuda_context & ctx, ggml_tensor * dst) {
    const ggml_tensor * src0 = dst->src[0];
    const float * src0_d = (const float *)src0->data;
    float * dst_d = (float *)dst->data;
    cudaStream_t stream = ctx.stream();

    GGML_ASSERT(src0->type == GGML_TYPE_F32);
    GGML_ASSERT(dst->type == GGML_TYPE_F32);
    GGML_ASSERT(src0->ne[3] == 1 && dst->ne[3] == 1); // just 3D tensors

    pad_f32_cuda(src0_d, dst_d,
        src0->ne[0], src0->ne[1], src0->ne[2], src0->ne[3],
        dst->ne[0], dst->ne[1], dst->ne[2], dst->ne[3], stream);
}
