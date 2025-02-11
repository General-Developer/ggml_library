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
#include "tsembd.cuh"

static __global__ void timestep_embedding_f32(const float * timesteps, float * dst, const int nb1, const int dim, const int max_period) {
    // blockIDx.y: idx of timesteps->ne[0]
    // blockIDx.x: idx of ((dim + 1) / 2) / BLOCK_SIZE
    int i = blockIdx.y;
    int j = threadIdx.x + blockIdx.x * blockDim.x;
    float * embed_data = (float *)((char *)dst +  i*nb1);

    if (dim % 2 != 0 && j == ((dim + 1) / 2)) {
        embed_data[dim] = 0.f;
    }

    int half = dim / 2;
    if (j >= half) {
        return;
    }

    float timestep = timesteps[i];
    float freq = (float)expf(-logf(max_period) * j / half);
    float arg = timestep * freq;
    embed_data[j] = cosf(arg);
    embed_data[j + half] = sinf(arg);
}

static void timestep_embedding_f32_cuda(const float * x, float * dst, const int ne00, const int nb1,
                                        const int dim, const int max_period, cudaStream_t stream) {
    int half_ceil = (dim + 1) / 2;
    int num_blocks = (half_ceil + CUDA_TIMESTEP_EMBEDDING_BLOCK_SIZE - 1) / CUDA_TIMESTEP_EMBEDDING_BLOCK_SIZE;
    dim3 gridDim(num_blocks, ne00, 1);
    timestep_embedding_f32<<<gridDim, CUDA_TIMESTEP_EMBEDDING_BLOCK_SIZE, 0, stream>>>(x, dst, nb1, dim, max_period);
}

void ggml_cuda_op_timestep_embedding(ggml_backend_cuda_context & ctx, ggml_tensor * dst) {
    const ggml_tensor * src0 = dst->src[0];
    const float * src0_d = (const float *)src0->data;
    float * dst_d = (float *)dst->data;
    cudaStream_t stream = ctx.stream();

    GGML_ASSERT(src0->type == GGML_TYPE_F32);
    GGML_ASSERT(dst->type == GGML_TYPE_F32);

    const int dim = dst->op_params[0];
    const int max_period = dst->op_params[1];

    timestep_embedding_f32_cuda(src0_d, dst_d, src0->ne[0], dst->nb[1], dim, max_period, stream);
}
