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
var factory = require('../bindings/javascript/whisper.js')

factory().then(function(whisper) {
    var fs = require('fs');

    // to avoid reading WAV files and depending on some 3rd-party package, we read
    // 32-bit float PCM directly. to genereate it:
    //
    //   $ ffmpeg -i samples/jfk.wav -f f32le -acodec pcm_f32le samples/jfk.pcmf32
    //
    let fname_wav   = "../samples/jfk.pcmf32";
    let fname_model = "../models/ggml-base.en.bin";

    // init whisper
    {
        // read binary data from file
        var model_data = fs.readFileSync(fname_model);
        if (model_data == null) {
            console.log("whisper: failed to read model file");
            process.exit(1);
        }

        // write binary data to WASM memory
        whisper.FS_createDataFile("/", "whisper.bin", model_data, true, true);

        // init the model
        var ret = whisper.init("whisper.bin");
        if (ret == false) {
            console.log('whisper: failed to init');
            process.exit(1);
        }
    }

    // transcribe wav file
    {
        // read raw binary data
        var pcm_data = fs.readFileSync(fname_wav);
        if (pcm_data == null) {
            console.log("whisper: failed to read wav file");
            process.exit(1);
        }

        // convert to 32-bit float array
        var pcm = new Float32Array(pcm_data.buffer);

        // transcribe
        var ret = whisper.full_default(pcm, "en", false);
        if (ret != 0) {
            console.log("whisper: failed to transcribe");
            process.exit(1);
        }
    }

    // free memory
    {
        whisper.free();
    }
});
