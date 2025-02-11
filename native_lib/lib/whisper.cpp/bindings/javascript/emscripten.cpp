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
// This is the Javascript API of whisper.cpp
//
// Very crude at the moment.
// Feel free to contribute and make this better!
//
// See the tests/test-whisper.js for sample usage
//

#include "whisper.h"

#include <emscripten.h>
#include <emscripten/bind.h>

#include <thread>
#include <vector>

struct whisper_context * g_context;

EMSCRIPTEN_BINDINGS(whisper) {
    emscripten::function("init", emscripten::optional_override([](const std::string & path_model) {
        if (g_context == nullptr) {
            g_context = whisper_init_from_file_with_params(path_model.c_str(), whisper_context_default_params());
            if (g_context != nullptr) {
                return true;
            } else {
                return false;
            }
        }

        return false;
    }));

    emscripten::function("free", emscripten::optional_override([]() {
        if (g_context) {
            whisper_free(g_context);
            g_context = nullptr;
        }
    }));

    emscripten::function("full_default", emscripten::optional_override([](const emscripten::val & audio, const std::string & lang, bool translate) {
        if (g_context == nullptr) {
            return -1;
        }

        struct whisper_full_params params = whisper_full_default_params(whisper_sampling_strategy::WHISPER_SAMPLING_GREEDY);

        params.print_realtime   = true;
        params.print_progress   = false;
        params.print_timestamps = true;
        params.print_special    = false;
        params.translate        = translate;
        params.language         = whisper_is_multilingual(g_context) ? lang.c_str() : "en";
        params.n_threads        = std::min(8, (int) std::thread::hardware_concurrency());
        params.offset_ms        = 0;

        std::vector<float> pcmf32;
        const int n = audio["length"].as<int>();

        emscripten::val heap = emscripten::val::module_property("HEAPU8");
        emscripten::val memory = heap["buffer"];

        pcmf32.resize(n);

        emscripten::val memoryView = audio["constructor"].new_(memory, reinterpret_cast<uintptr_t>(pcmf32.data()), n);
        memoryView.call<void>("set", audio);

        // print system information
        {
            printf("\n");
            printf("system_info: n_threads = %d / %d | %s\n",
                    params.n_threads, std::thread::hardware_concurrency(), whisper_print_system_info());

            printf("\n");
            printf("%s: processing %d samples, %.1f sec, %d threads, %d processors, lang = %s, task = %s ...\n",
                    __func__, int(pcmf32.size()), float(pcmf32.size())/WHISPER_SAMPLE_RATE,
                    params.n_threads, 1,
                    params.language,
                    params.translate ? "translate" : "transcribe");

            printf("\n");
        }

        // run whisper
        {
            whisper_reset_timings(g_context);
            whisper_full(g_context, params, pcmf32.data(), pcmf32.size());
            whisper_print_timings(g_context);
        }

        return 0;
    }));
}
