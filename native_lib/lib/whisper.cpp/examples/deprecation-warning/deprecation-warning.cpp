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
// Warns users that this filename was deprecated, and provides a link for more information.

#include <cstdio>
#include <string>

// Main
int main(int argc, char** argv) {
    std::string filename = "main";
    if (argc >= 1) {
        filename = argv[0];
    }

    // Get only the program name from the full path
    size_t pos = filename.find_last_of("/\\");
    if (pos != std::string::npos) {
        filename = filename.substr(pos+1);
    }

    // Append "whisper-" to the beginning of filename to get the replacemnt filename
    std::string replacement_filename = "whisper-" + filename;

    // The exception is if the filename is "main", then our replacement filename is "whisper-cli"
    if (filename == "main") {
        replacement_filename = "whisper-cli";
    }

    if (filename == "main.exe") {
        replacement_filename = "whisper-cli.exe";
    }

    fprintf(stdout, "\n");
    fprintf(stdout, "WARNING: The binary '%s' is deprecated.\n", filename.c_str());
    fprintf(stdout, " Please use '%s' instead.\n", replacement_filename.c_str());
    fprintf(stdout, " See https://github.com/ggerganov/whisper.cpp/tree/master/examples/deprecation-warning/README.md for more information.\n");
    fprintf(stdout, "\n");

    return EXIT_FAILURE;
}
