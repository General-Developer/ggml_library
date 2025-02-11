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
#include "whisper.h"

#include <emscripten.h>
#include <emscripten/bind.h>

#include <cmath>
#include <string>
#include <thread>
#include <vector>

constexpr int N_THREAD = 8;

// TODO: get rid of this vector of contexts - bad idea in the first place
std::vector<struct whisper_context *> g_contexts(4, nullptr);

std::thread g_worker;

void bench_main(size_t index) {
    const int n_threads = std::min(N_THREAD, (int) std::thread::hardware_concurrency());

    // whisper context
    auto & ctx = g_contexts[index];

    fprintf(stderr, "%s: running benchmark with %d threads - please wait...\n", __func__, n_threads);

    const int n_mels = whisper_model_n_mels(ctx);

    if (int ret = whisper_set_mel(ctx, nullptr, 0, n_mels)) {
        fprintf(stderr, "error: failed to set mel: %d\n", ret);
        return;
    }

    {
        fprintf(stderr, "\n");
        fprintf(stderr, "system_info: n_threads = %d / %d | %s\n", n_threads, std::thread::hardware_concurrency(), whisper_print_system_info());
    }

    if (int ret = whisper_encode(ctx, 0, n_threads) != 0) {
        fprintf(stderr, "error: failed to encode model: %d\n", ret);
        return;
    }

    whisper_print_timings(ctx);

    fprintf(stderr, "\n");
    fprintf(stderr, "If you wish, you can submit these results here:\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "  https://github.com/ggerganov/whisper.cpp/issues/89\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "Please include the following information:\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "  - CPU model\n");
    fprintf(stderr, "  - Operating system\n");
    fprintf(stderr, "  - Browser\n");
    fprintf(stderr, "\n");
}

EMSCRIPTEN_BINDINGS(bench) {
    emscripten::function("init", emscripten::optional_override([](const std::string & path_model) {
        for (size_t i = 0; i < g_contexts.size(); ++i) {
            if (g_contexts[i] == nullptr) {
                g_contexts[i] = whisper_init_from_file_with_params(path_model.c_str(), whisper_context_default_params());
                if (g_contexts[i] != nullptr) {
                    if (g_worker.joinable()) {
                        g_worker.join();
                    }
                    g_worker = std::thread([i]() {
                        bench_main(i);
                    });

                    return i + 1;
                } else {
                    return (size_t) 0;
                }
            }
        }

        return (size_t) 0;
    }));

    emscripten::function("free", emscripten::optional_override([](size_t index) {
        if (index < g_contexts.size()) {
            whisper_free(g_contexts[index]);
            g_contexts[index] = nullptr;
        }
    }));
}
