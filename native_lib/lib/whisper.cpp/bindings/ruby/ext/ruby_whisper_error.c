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
#include <ruby.h>

extern VALUE eError;

VALUE ruby_whisper_error_initialize(VALUE self, VALUE code)
{
  const int c_code = NUM2INT(code);
  const char *raw_message;
  switch (c_code) {
  case -2:
    raw_message = "failed to compute log mel spectrogram";
    break;
  case -3:
    raw_message = "failed to auto-detect language";
    break;
  case -4:
    raw_message = "too many decoders requested";
    break;
  case -5:
    raw_message = "audio_ctx is larger than the maximum allowed";
    break;
  case -6:
    raw_message = "failed to encode";
    break;
  case -7:
    raw_message = "whisper_kv_cache_init() failed for self-attention cache";
    break;
  case -8:
    raw_message = "failed to decode";
    break;
  case -9:
    raw_message = "failed to decode";
    break;
  default:
    raw_message = "unknown error";
    break;
  }
  const VALUE message = rb_str_new2(raw_message);
  rb_call_super(1, &message);
  rb_iv_set(self, "@code", code);

  return self;
}

void
init_ruby_whisper_error(VALUE *mWhisper)
{
  eError = rb_define_class_under(*mWhisper, "Error", rb_eStandardError);

  rb_define_attr(eError, "code", true, false);
  rb_define_method(eError, "initialize", ruby_whisper_error_initialize, 1);
}
