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
#include <ruby/memory_view.h>
#include <ruby/encoding.h>

static VALUE
jfk_reader_initialize(VALUE self, VALUE audio_path)
{
  rb_iv_set(self, "audio_path", audio_path);
  return Qnil;
}

static bool
jfk_reader_get_memory_view(const VALUE obj, rb_memory_view_t *view, int flags)
{
  VALUE audio_path = rb_iv_get(obj, "audio_path");
  const char *audio_path_str = StringValueCStr(audio_path);
  const int n_samples = 176000;
  float *data = (float *)malloc(n_samples * sizeof(float));
  short *samples = (short *)malloc(n_samples * sizeof(short));
  FILE *file = fopen(audio_path_str, "rb");

  fseek(file, 78, SEEK_SET);
  fread(samples, sizeof(short), n_samples, file);
  fclose(file);
  for (int i = 0; i < n_samples; i++) {
    data[i] = samples[i]/32768.0;
  }

  view->obj = obj;
  view->data = (void *)data;
  view->byte_size = sizeof(float) * n_samples;
  view->readonly = true;
  view->format = "f";
  view->item_size = sizeof(float);
  view->item_desc.components = NULL;
  view->item_desc.length = 0;
  view->ndim = 1;
  view->shape = NULL;
  view->sub_offsets = NULL;
  view->private_data = NULL;

  return true;
}

static bool
jfk_reader_release_memory_view(const VALUE obj, rb_memory_view_t *view)
{
  return true;
}

static bool
jfk_reader_memory_view_available_p(const VALUE obj)
{
  return true;
}

static const rb_memory_view_entry_t jfk_reader_view_entry = {
  jfk_reader_get_memory_view,
  jfk_reader_release_memory_view,
  jfk_reader_memory_view_available_p
};

void Init_jfk_reader(void)
{
  VALUE cJFKReader = rb_define_class("JFKReader", rb_cObject);
  rb_memory_view_register(cJFKReader, &jfk_reader_view_entry);
  rb_define_method(cJFKReader, "initialize", jfk_reader_initialize, 1);
}
