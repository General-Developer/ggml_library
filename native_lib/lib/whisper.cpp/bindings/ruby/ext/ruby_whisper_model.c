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
#include "ruby_whisper.h"

extern VALUE cModel;

static void rb_whisper_model_mark(ruby_whisper_model *rwm) {
  rb_gc_mark(rwm->context);
}

static VALUE ruby_whisper_model_allocate(VALUE klass) {
  ruby_whisper_model *rwm;
  rwm = ALLOC(ruby_whisper_model);
  return Data_Wrap_Struct(klass, rb_whisper_model_mark, RUBY_DEFAULT_FREE, rwm);
}

VALUE rb_whisper_model_initialize(VALUE context) {
  ruby_whisper_model *rwm;
  const VALUE model = ruby_whisper_model_allocate(cModel);
  Data_Get_Struct(model, ruby_whisper_model, rwm);
  rwm->context = context;
  return model;
};

/*
 * call-seq:
 *   n_vocab -> Integer
 */
static VALUE
ruby_whisper_model_n_vocab(VALUE self)
{
  ruby_whisper_model *rwm;
  Data_Get_Struct(self, ruby_whisper_model, rwm);
  ruby_whisper *rw;
  Data_Get_Struct(rwm->context, ruby_whisper, rw);
  return INT2NUM(whisper_model_n_vocab(rw->context));
}

/*
 * call-seq:
 *   n_audio_ctx -> Integer
 */
static VALUE
ruby_whisper_model_n_audio_ctx(VALUE self)
{
  ruby_whisper_model *rwm;
  Data_Get_Struct(self, ruby_whisper_model, rwm);
  ruby_whisper *rw;
  Data_Get_Struct(rwm->context, ruby_whisper, rw);
  return INT2NUM(whisper_model_n_audio_ctx(rw->context));
}

/*
 * call-seq:
 *   n_audio_state -> Integer
 */
static VALUE
ruby_whisper_model_n_audio_state(VALUE self)
{
  ruby_whisper_model *rwm;
  Data_Get_Struct(self, ruby_whisper_model, rwm);
  ruby_whisper *rw;
  Data_Get_Struct(rwm->context, ruby_whisper, rw);
  return INT2NUM(whisper_model_n_audio_state(rw->context));
}

/*
 * call-seq:
 *   n_audio_head -> Integer
 */
static VALUE
ruby_whisper_model_n_audio_head(VALUE self)
{
  ruby_whisper_model *rwm;
  Data_Get_Struct(self, ruby_whisper_model, rwm);
  ruby_whisper *rw;
  Data_Get_Struct(rwm->context, ruby_whisper, rw);
  return INT2NUM(whisper_model_n_audio_head(rw->context));
}

/*
 * call-seq:
 *   n_audio_layer -> Integer
 */
static VALUE
ruby_whisper_model_n_audio_layer(VALUE self)
{
  ruby_whisper_model *rwm;
  Data_Get_Struct(self, ruby_whisper_model, rwm);
  ruby_whisper *rw;
  Data_Get_Struct(rwm->context, ruby_whisper, rw);
  return INT2NUM(whisper_model_n_audio_layer(rw->context));
}

/*
 * call-seq:
 *   n_text_ctx -> Integer
 */
static VALUE
ruby_whisper_model_n_text_ctx(VALUE self)
{
  ruby_whisper_model *rwm;
  Data_Get_Struct(self, ruby_whisper_model, rwm);
  ruby_whisper *rw;
  Data_Get_Struct(rwm->context, ruby_whisper, rw);
  return INT2NUM(whisper_model_n_text_ctx(rw->context));
}

/*
 * call-seq:
 *   n_text_state -> Integer
 */
static VALUE
ruby_whisper_model_n_text_state(VALUE self)
{
  ruby_whisper_model *rwm;
  Data_Get_Struct(self, ruby_whisper_model, rwm);
  ruby_whisper *rw;
  Data_Get_Struct(rwm->context, ruby_whisper, rw);
  return INT2NUM(whisper_model_n_text_state(rw->context));
}

/*
 * call-seq:
 *   n_text_head -> Integer
 */
static VALUE
ruby_whisper_model_n_text_head(VALUE self)
{
  ruby_whisper_model *rwm;
  Data_Get_Struct(self, ruby_whisper_model, rwm);
  ruby_whisper *rw;
  Data_Get_Struct(rwm->context, ruby_whisper, rw);
  return INT2NUM(whisper_model_n_text_head(rw->context));
}

/*
 * call-seq:
 *   n_text_layer -> Integer
 */
static VALUE
ruby_whisper_model_n_text_layer(VALUE self)
{
  ruby_whisper_model *rwm;
  Data_Get_Struct(self, ruby_whisper_model, rwm);
  ruby_whisper *rw;
  Data_Get_Struct(rwm->context, ruby_whisper, rw);
  return INT2NUM(whisper_model_n_text_layer(rw->context));
}

/*
 * call-seq:
 *   n_mels -> Integer
 */
static VALUE
ruby_whisper_model_n_mels(VALUE self)
{
  ruby_whisper_model *rwm;
  Data_Get_Struct(self, ruby_whisper_model, rwm);
  ruby_whisper *rw;
  Data_Get_Struct(rwm->context, ruby_whisper, rw);
  return INT2NUM(whisper_model_n_mels(rw->context));
}

/*
 * call-seq:
 *   ftype -> Integer
 */
static VALUE
ruby_whisper_model_ftype(VALUE self)
{
  ruby_whisper_model *rwm;
  Data_Get_Struct(self, ruby_whisper_model, rwm);
  ruby_whisper *rw;
  Data_Get_Struct(rwm->context, ruby_whisper, rw);
  return INT2NUM(whisper_model_ftype(rw->context));
}

/*
 * call-seq:
 *   type -> String
 */
static VALUE
ruby_whisper_model_type(VALUE self)
{
  ruby_whisper_model *rwm;
  Data_Get_Struct(self, ruby_whisper_model, rwm);
  ruby_whisper *rw;
  Data_Get_Struct(rwm->context, ruby_whisper, rw);
  return rb_str_new2(whisper_model_type_readable(rw->context));
}

void
init_ruby_whisper_model(VALUE *mWhisper)
{
  cModel = rb_define_class_under(*mWhisper, "Model", rb_cObject);

  rb_define_alloc_func(cModel, ruby_whisper_model_allocate);
  rb_define_method(cModel, "n_vocab", ruby_whisper_model_n_vocab, 0);
  rb_define_method(cModel, "n_audio_ctx", ruby_whisper_model_n_audio_ctx, 0);
  rb_define_method(cModel, "n_audio_state", ruby_whisper_model_n_audio_state, 0);
  rb_define_method(cModel, "n_audio_head", ruby_whisper_model_n_audio_head, 0);
  rb_define_method(cModel, "n_audio_layer", ruby_whisper_model_n_audio_layer, 0);
  rb_define_method(cModel, "n_text_ctx", ruby_whisper_model_n_text_ctx, 0);
  rb_define_method(cModel, "n_text_state", ruby_whisper_model_n_text_state, 0);
  rb_define_method(cModel, "n_text_head", ruby_whisper_model_n_text_head, 0);
  rb_define_method(cModel, "n_text_layer", ruby_whisper_model_n_text_layer, 0);
  rb_define_method(cModel, "n_mels", ruby_whisper_model_n_mels, 0);
  rb_define_method(cModel, "ftype", ruby_whisper_model_ftype, 0);
  rb_define_method(cModel, "type", ruby_whisper_model_type, 0);
}
