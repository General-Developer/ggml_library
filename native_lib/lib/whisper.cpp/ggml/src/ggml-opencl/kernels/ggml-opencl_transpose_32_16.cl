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
// 32-bit transpose, loading/storing a 4x4 tile of elements
// Only used for activations
// converts to FP16
// also adds zero padding for non multiple of 8 prompt lengths
#pragma OPENCL EXTENSION cl_khr_fp16 : enable

kernel void kernel_transpose_32_16(__read_only image1d_buffer_t input, __write_only image1d_buffer_t output, const uint rows, const uint cols, const uint padded_rows) {

    const int i = get_global_id(0);
    const int j = get_global_id(1);
    const int i_2 = i<<2;
    const int j_2 = j<<2;
    half4 temp0 = {0,0,0,0}; // initialize outputs to 0
    half4 temp1 = {0,0,0,0};
    half4 temp2 = {0,0,0,0};
    half4 temp3 = {0,0,0,0};

    if((j_2+0)*cols+i*4+3 < rows*cols*16){ // only load from a valid location. Otherwise keep register data as 0
        temp0 = read_imageh(input, (j_2+0)*cols+i);
    }
    if((j_2+1)*cols+i*4+3 < rows*cols*16){
        temp1 = read_imageh(input, (j_2+1)*cols+i);
    }
    if((j_2+2)*cols+i*4+3 < rows*cols*16){
        temp2 = read_imageh(input, (j_2+2)*cols+i);
    }
    if((j_2+3)*cols+i*4+3 < rows*cols*16){
        temp3 = read_imageh(input, (j_2+3)*cols+i);
    }

    write_imageh(output, (i_2+0)*padded_rows+j, (half4)(temp0.s0, temp1.s0, temp2.s0, temp3.s0)); // no conditionals for output, includes zero padding
    write_imageh(output, (i_2+1)*padded_rows+j, (half4)(temp0.s1, temp1.s1, temp2.s1, temp3.s1));
    write_imageh(output, (i_2+2)*padded_rows+j, (half4)(temp0.s2, temp1.s2, temp2.s2, temp3.s2));
    write_imageh(output, (i_2+3)*padded_rows+j, (half4)(temp0.s3, temp1.s3, temp2.s3, temp3.s3));
}
