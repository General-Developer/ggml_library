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
//------------------------------------------------------------------------------
// This file is contains additional kernels for data conversion.
// These kernels are used when loading the model, so its performance is less
// important.
//------------------------------------------------------------------------------
#ifdef cl_khr_fp16
#pragma OPENCL EXTENSION cl_khr_fp16 : enable
#elif defined(cl_amd_fp16)
#pragma OPENCL EXTENSION cl_amd_fp16 : enable
#else
#error "Half precision floating point not supportedby OpenCL implementation on your device."
#endif

#ifdef cl_khr_subgroups
#pragma OPENCL EXTENSION cl_khr_subgroups : enable
#elif defined(cl_intel_subgroups)
#pragma OPENCL EXTENSION cl_intel_subgroups : enable
#else
#error "Subgroup not supported on your device."
#endif

#ifdef cl_intel_required_subgroup_size
// Always use subgroup size of 32 on Intel.
#pragma OPENCL EXTENSION cl_intel_required_subgroup_size : enable
#define INTEL_GPU 1
#define REQD_SUBGROUP_SIZE_16 __attribute__((intel_reqd_sub_group_size(16)))
#define REQD_SUBGROUP_SIZE_32 __attribute__((intel_reqd_sub_group_size(32)))
#elif defined(cl_qcom_reqd_sub_group_size)
// Always use subgroups size of 64 on Adreno.
#pragma OPENCL EXTENSION cl_qcom_reqd_sub_group_size : enable
#define ADRENO_GPU 1
#define REQD_SUBGROUP_SIZE_64  __attribute__((qcom_reqd_sub_group_size("half")))
#define REQD_SUBGROUP_SIZE_128 __attribute__((qcom_reqd_sub_group_size("full")))
#else
// TODO: do not know how to choose subgroup size on other GPUs.
#error "Selecting subgroup size is not supported on your device."
#endif

#define QK4_0                   32
#define QR4_0                   2
#define QK4_1                   32
#define QR4_1                   2
#define QK5_0                   32
#define QR5_0                   2
#define QK5_1                   32
#define QR5_1                   2
#define QK8_0                   32
#define QR8_0                   1
#define QK_K                    256
#define K_QUANTS_PER_ITERATION  2

typedef char int8_t;
typedef uchar uint8_t;
typedef short int16_t;
typedef ushort uint16_t;
typedef int int32_t;
typedef uint uint32_t;

//------------------------------------------------------------------------------
// block_q4_0
//------------------------------------------------------------------------------
struct block_q4_0
{
    half d;
    uint8_t qs[QK4_0 / 2];
};

//------------------------------------------------------------------------------
// mul_vec_q_n_f32_flat_noshuffle
//
// This variation uses flat arrays (struct of arrays, SOA) representation for
// quant tensors. It also uses non shuffled bit order for weights.
//
// The shuffled version is kept in the original file because moving it here
// seems to result in worse performance for adreno.
//------------------------------------------------------------------------------

kernel void kernel_convert_block_q4_0_noshuffle(
    global struct block_q4_0 * src0,
    global uchar * dst_q,
    global half  * dst_d
) {
    global struct block_q4_0 * b = (global struct block_q4_0 *) src0 + get_global_id(0);
    global uchar * q = (global uchar *) dst_q + QK4_0/2*get_global_id(0);
    global half  * d = (global half *) dst_d + get_global_id(0);

    *d = b->d;
    for (int i = 0; i < QK4_0/4; ++i) {
        uchar x0 = b->qs[2*i + 0];
        uchar x1 = b->qs[2*i + 1];

        q[i + 0      ] = convert_uchar(x0 & 0x0F) | convert_uchar((x1 & 0x0F) << 4);
        q[i + QK4_0/4] = convert_uchar((x0 & 0xF0) >> 4) | convert_uchar(x1 & 0xF0);

#ifdef ADRENO_GPU
        // Workaround for adreno - must have the following printf statement for
        // the kernel to work properly. Otherwise it produces incorrect result.
        // convert_uchar above also seems necessary.
        // Compare against a large number so that it does not print anything.
        // get_sub_group_local_id() also works.
        if (get_global_id(0) == 65536*4096) {
            printf("%04x - %02x\n", *(global ushort*)d, ((x0 & 0xF0) >> 4) | (x1 & 0xF0));
        }
#endif
    }
}
