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
const path = require("path");
const { whisper } = require(path.join(
  __dirname,
  "../../build/Release/addon.node"
));
const { promisify } = require("util");

const whisperAsync = promisify(whisper);

const whisperParams = {
  language: "en",
  model: path.join(__dirname, "../../models/ggml-base.en.bin"),
  fname_inp: path.join(__dirname, "../../samples/jfk.wav"),
  use_gpu: true,
  flash_attn: false,
  no_prints: true,
  comma_in_time: false,
  translate: true,
  no_timestamps: false,
  audio_ctx: 0,
  max_len: 0,
};

const arguments = process.argv.slice(2);
const params = Object.fromEntries(
  arguments.reduce((pre, item) => {
    if (item.startsWith("--")) {
      const [key, value] = item.slice(2).split("=");
      if (key === "audio_ctx") {
        whisperParams[key] = parseInt(value);
      } else {
        whisperParams[key] = value;
      }
      return pre;
    }
    return pre;
  }, [])
);

for (const key in params) {
  if (whisperParams.hasOwnProperty(key)) {
    whisperParams[key] = params[key];
  }
}

console.log("whisperParams =", whisperParams);

whisperAsync(whisperParams).then((result) => {
  console.log();
  console.log(result);
});
