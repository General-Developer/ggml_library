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

extern VALUE cSegment;

static void
rb_whisper_segment_mark(ruby_whisper_segment *rws)
{
  rb_gc_mark(rws->context);
}

VALUE
ruby_whisper_segment_allocate(VALUE klass)
{
  ruby_whisper_segment *rws;
  rws = ALLOC(ruby_whisper_segment);
  return Data_Wrap_Struct(klass, rb_whisper_segment_mark, RUBY_DEFAULT_FREE, rws);
}

VALUE
rb_whisper_segment_initialize(VALUE context, int index)
{
  ruby_whisper_segment *rws;
  const VALUE segment = ruby_whisper_segment_allocate(cSegment);
  Data_Get_Struct(segment, ruby_whisper_segment, rws);
  rws->context = context;
  rws->index = index;
  return segment;
};

/*
 * Start time in milliseconds.
 *
 * call-seq:
 *   start_time -> Integer
 */
static VALUE
ruby_whisper_segment_get_start_time(VALUE self)
{
  ruby_whisper_segment *rws;
  Data_Get_Struct(self, ruby_whisper_segment, rws);
  ruby_whisper *rw;
  Data_Get_Struct(rws->context, ruby_whisper, rw);
  const int64_t t0 = whisper_full_get_segment_t0(rw->context, rws->index);
  // able to multiply 10 without overflow because to_timestamp() in whisper.cpp does it
  return INT2NUM(t0 * 10);
}

/*
 * End time in milliseconds.
 *
 * call-seq:
 *   end_time -> Integer
 */
static VALUE
ruby_whisper_segment_get_end_time(VALUE self)
{
  ruby_whisper_segment *rws;
  Data_Get_Struct(self, ruby_whisper_segment, rws);
  ruby_whisper *rw;
  Data_Get_Struct(rws->context, ruby_whisper, rw);
  const int64_t t1 = whisper_full_get_segment_t1(rw->context, rws->index);
  // able to multiply 10 without overflow because to_timestamp() in whisper.cpp does it
  return INT2NUM(t1 * 10);
}

/*
 * Whether the next segment is predicted as a speaker turn.
 *
 * call-seq:
 *   speaker_turn_next? -> bool
 */
static VALUE
ruby_whisper_segment_get_speaker_turn_next(VALUE self)
{
  ruby_whisper_segment *rws;
  Data_Get_Struct(self, ruby_whisper_segment, rws);
  ruby_whisper *rw;
  Data_Get_Struct(rws->context, ruby_whisper, rw);
  return whisper_full_get_segment_speaker_turn_next(rw->context, rws->index) ? Qtrue : Qfalse;
}

/*
 * call-seq:
 *   text -> String
 */
static VALUE
ruby_whisper_segment_get_text(VALUE self)
{
  ruby_whisper_segment *rws;
  Data_Get_Struct(self, ruby_whisper_segment, rws);
  ruby_whisper *rw;
  Data_Get_Struct(rws->context, ruby_whisper, rw);
  const char * text = whisper_full_get_segment_text(rw->context, rws->index);
  return rb_str_new2(text);
}

/*
 * call-seq:
 *   no_speech_prob -> Float
 */
static VALUE
ruby_whisper_segment_get_no_speech_prob(VALUE self)
{
  ruby_whisper_segment *rws;
  Data_Get_Struct(self, ruby_whisper_segment, rws);
  ruby_whisper *rw;
  Data_Get_Struct(rws->context, ruby_whisper, rw);
  return DBL2NUM(whisper_full_get_segment_no_speech_prob(rw->context, rws->index));
}

void
init_ruby_whisper_segment(VALUE *mWhisper, VALUE *cContext)
{
  cSegment  = rb_define_class_under(*mWhisper, "Segment", rb_cObject);

  rb_define_alloc_func(cSegment, ruby_whisper_segment_allocate);
  rb_define_method(cSegment, "start_time", ruby_whisper_segment_get_start_time, 0);
  rb_define_method(cSegment, "end_time", ruby_whisper_segment_get_end_time, 0);
  rb_define_method(cSegment, "speaker_next_turn?", ruby_whisper_segment_get_speaker_turn_next, 0);
  rb_define_method(cSegment, "text", ruby_whisper_segment_get_text, 0);
  rb_define_method(cSegment, "no_speech_prob", ruby_whisper_segment_get_no_speech_prob, 0);
}
