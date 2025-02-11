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
#include "openvino/whisper-openvino-encoder.h"
#include "ggml.h"
#include <openvino/openvino.hpp>
#include <iostream>

struct whisper_openvino_context {
    ov::InferRequest inferRequest;
};

struct whisper_openvino_context * whisper_openvino_init(const char* path_model,
    const char* device,
    const char* cache_dir)
{
    if (!path_model || !device) {
        fprintf(stderr, "%s: path_model and/or device is null\n", __func__);
        return nullptr;
    }

    fprintf(stderr, "%s: path_model = %s, device = %s, cache_dir = %s\n",
        __func__, path_model, device, cache_dir ? cache_dir : "(not set)");

	whisper_openvino_context *context = new whisper_openvino_context;
    try {
        ov::Core core;

        if (cache_dir) {
            // enables caching of device-specific 'blobs' during core.compile_model
            // routine. This speeds up calls to compile_model for successive runs.
            core.set_property(ov::cache_dir(cache_dir));
        }

        //Read the OpenVINO encoder IR (.xml/.bin) from disk, producing an ov::Model object.
        std::shared_ptr<ov::Model> model = core.read_model(path_model);

        // Produce a compiled-model object, given the device ("CPU", "GPU", etc.)
        auto compiledModel = core.compile_model(model, device);

        // From the compiled model object, create an infer request. This is the thing that we
        //  we will use later on to trigger inference execution.
        context->inferRequest = compiledModel.create_infer_request();
    }
    catch (const std::exception& error) {
        std::cout << "in openvino encoder compile routine: exception: " << error.what() << std::endl;
        delete context;
        context = nullptr;
    }

    return context;
}

void whisper_openvino_free(struct whisper_openvino_context * ctx) {
    if( ctx ) {
        delete ctx;
    }
}

int whisper_openvino_encode(
    whisper_openvino_context* ctx,
    ggml_tensor* mel,
    ggml_tensor* out) {

    if (!ctx || !mel || !out) {
        fprintf(stderr, "%s: Error! ctx / mel / out is null\n", __func__);
        return 0;
    }

    if (ggml_n_dims(mel) != 2) {
        fprintf(stderr, "%s: Error! mel ggml_tensor expected to have n_dims=2, but it has n_dims=%d\n",
            __func__, ggml_n_dims(mel));
        return 0;
    }

    if (ggml_n_dims(out) != 2) {
        fprintf(stderr, "%s: Error! out ggml_tensor expected to have n_dims=2, but it has n_dims=%d\n",
            __func__, ggml_n_dims(out));
        return 0;
    }

    try {

        //wrap the passed-in mel ggml_tensor as an OpenVINO Tensor object, and set as input tensor to infer request
        {
            // note, we populate shape & stride dimensions in opposite order from how they are listed in ne / nb arrays
            ov::Shape input_shape = { 1, (unsigned long long)mel->ne[1], (unsigned long long)mel->ne[0] };
            ov::Strides input_strides = { mel->nb[2], mel->nb[1], mel->nb[0] };
            ov::Tensor input_tensor(ov::element::f32, input_shape, mel->data, input_strides);
            ctx->inferRequest.set_input_tensor(input_tensor);
        }

        //wrap the passed-in out ggml_tensor as an OpenVINO Tensor object, and set as output tensor to infer request
        {
            // note, we populate shape & stride dimensions in opposite order from how they are listed in ne / nb arrays
            ov::Shape output_shape = { 1, (unsigned long long)out->ne[1], (unsigned long long)out->ne[0] };
            ov::Strides output_strides = { out->nb[2], out->nb[1], out->nb[0] };
            ov::Tensor out_tensor(ov::element::f32, output_shape, out->data, output_strides);
            ctx->inferRequest.set_output_tensor(out_tensor);
        }

        //run inference
        ctx->inferRequest.infer();
    }
    catch (const std::exception& error) {
        std::cout << "in openvino encode inference execution routine: exception: " << error.what() << std::endl;
        return 0;
    }

    return 1;
}
