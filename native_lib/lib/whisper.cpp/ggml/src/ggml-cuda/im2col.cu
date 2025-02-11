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
#include "im2col.cuh"

template <typename T>
static  __global__ void im2col_kernel(
        const float * x, T * dst, int64_t batch_offset,
        int64_t offset_delta, int64_t IC, int64_t IW, int64_t IH, int64_t OH, int64_t OW, int64_t KW, int64_t KH, int64_t pelements, int64_t CHW,
        int s0, int s1, int p0, int p1, int d0, int d1) {
    const int64_t i = threadIdx.x + blockIdx.x * blockDim.x;
    if (i >= pelements) {
        return;
    }

    const int64_t  ksize = OW * (KH > 1 ? KW : 1);
    const int64_t  kx = i / ksize;
    const int64_t  kd = kx * ksize;
    const int64_t  ky = (i - kd) / OW;
    const int64_t  ix = i % OW;

    const int64_t  oh = blockIdx.y;
    const int64_t  batch = blockIdx.z / IC;
    const int64_t  ic = blockIdx.z % IC;

    const int64_t iiw = ix * s0 + kx * d0 - p0;
    const int64_t iih = oh * s1 + ky * d1 - p1;

    const int64_t offset_dst =
        ((batch * OH + oh) * OW + ix) * CHW +
        (ic * (KW * KH) + ky * KW + kx);

    if (iih < 0 || iih >= IH || iiw < 0 || iiw >= IW) {
        dst[offset_dst] = 0.0f;
    } else {
        const int64_t offset_src = ic * offset_delta + batch * batch_offset;
        dst[offset_dst] = x[offset_src + iih * IW + iiw];
    }
}

template <typename T>
static void im2col_cuda(const float * x, T* dst,
    int64_t IW, int64_t IH, int64_t OW, int64_t OH, int64_t KW, int64_t KH, int64_t IC,
    int64_t batch, int64_t batch_offset, int64_t offset_delta,
    int s0,int s1,int p0,int p1,int d0,int d1, cudaStream_t stream) {
    const int parallel_elements = OW * KW * KH;
    const int num_blocks = (parallel_elements + CUDA_IM2COL_BLOCK_SIZE - 1) / CUDA_IM2COL_BLOCK_SIZE;
    dim3 block_nums(num_blocks, OH, batch * IC);
    im2col_kernel<<<block_nums, CUDA_IM2COL_BLOCK_SIZE, 0, stream>>>(x, dst, batch_offset, offset_delta, IC, IW, IH, OH, OW, KW, KH, parallel_elements, (IC * KH * KW), s0, s1, p0, p1, d0, d1);
}

static void im2col_cuda_f16(const float * x, half * dst,
    int64_t IW, int64_t IH, int64_t OW, int64_t OH, int64_t KW, int64_t KH, int64_t IC,
    int64_t batch, int64_t batch_offset, int64_t offset_delta,
    int s0,int s1,int p0,int p1,int d0,int d1, cudaStream_t stream) {

    im2col_cuda<half>(x, dst, IW, IH, OW, OH, KW, KH, IC, batch, batch_offset, offset_delta, s0, s1, p0, p1, d0, d1, stream);
}

static void im2col_cuda_f32(const float * x, float * dst,
    int64_t IW, int64_t IH, int64_t OW, int64_t OH, int64_t KW, int64_t KH, int64_t IC,
    int64_t batch, int64_t batch_offset, int64_t offset_delta,
    int s0,int s1,int p0,int p1,int d0,int d1, cudaStream_t stream) {

    im2col_cuda<float>(x, dst, IW, IH, OW, OH, KW, KH, IC, batch, batch_offset, offset_delta, s0, s1, p0, p1, d0, d1, stream);
}

void ggml_cuda_op_im2col(ggml_backend_cuda_context & ctx, ggml_tensor * dst) {
    const ggml_tensor * src0 = dst->src[0];
    const ggml_tensor * src1 = dst->src[1];
    const float * src1_d = (const float *)src1->data;
    float * dst_d = (float *)dst->data;
    cudaStream_t stream = ctx.stream();

    GGML_ASSERT(src1->type == GGML_TYPE_F32);
    GGML_ASSERT( dst->type == GGML_TYPE_F16 || dst->type == GGML_TYPE_F32);

    const int32_t s0 = ((const int32_t*)(dst->op_params))[0];
    const int32_t s1 = ((const int32_t*)(dst->op_params))[1];
    const int32_t p0 = ((const int32_t*)(dst->op_params))[2];
    const int32_t p1 = ((const int32_t*)(dst->op_params))[3];
    const int32_t d0 = ((const int32_t*)(dst->op_params))[4];
    const int32_t d1 = ((const int32_t*)(dst->op_params))[5];

    const bool is_2D = ((const int32_t*)(dst->op_params))[6] == 1;

    const int64_t IC = src1->ne[is_2D ? 2 : 1];
    const int64_t IH = is_2D ? src1->ne[1] : 1;
    const int64_t IW =         src1->ne[0];

    const int64_t KH = is_2D ? src0->ne[1] : 1;
    const int64_t KW =         src0->ne[0];

    const int64_t OH = is_2D ? dst->ne[2] : 1;
    const int64_t OW =         dst->ne[1];

    const size_t  delta_offset = src1->nb[is_2D ? 2 : 1] / 4; // nb is byte offset, src is type float32
    const int64_t batch        = src1->ne[is_2D ? 3 : 2];
    const size_t  batch_offset = src1->nb[is_2D ? 3 : 2] / 4; // nb is byte offset, src is type float32

    if(dst->type == GGML_TYPE_F16) {
        im2col_cuda_f16(src1_d, (half *) dst_d, IW, IH, OW, OH, KW, KH, IC, batch, batch_offset, delta_offset, s0, s1, p0, p1, d0, d1, stream);
    } else {
        im2col_cuda_f32(src1_d, (float *) dst_d, IW, IH, OW, OH, KW, KH, IC, batch, batch_offset, delta_offset, s0, s1, p0, p1, d0, d1, stream);
    }
}
