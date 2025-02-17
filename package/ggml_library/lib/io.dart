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
import 'dart:async';
import 'dart:ffi';

import 'package:ggml_library/core/ggml/ffi/bindings.dart';

import 'base.dart';
// import 'ffi/bindings.dart';

/// Check Out: https://www.youtube.com/@GENERAL_DEV
class GgmlLibrary extends GgmlLibraryBase {
  /// Check Out: https://www.youtube.com/@GENERAL_DEV
  GgmlLibrary({
    String? libraryGgmlPath,
  }) : super(
          libraryGgmlPath:
              libraryGgmlPath ?? GgmlLibraryBase.getLibraryWhisperPathDefault(),
        );

  /// Check Out: https://www.youtube.com/@GENERAL_DEV
  static late final GgmlLibrarySharedBindingsByGeneralDeveloper
      _ggmlLibrarySharedBindingsByGeneralDeveloper;

  /// Check Out: https://www.youtube.com/@GENERAL_DEV
  static bool _isEnsureInitialized = false;

  /// Check Out: https://www.youtube.com/@GENERAL_DEV
  @override
  Future<void> ensureInitialized() async {
    if (_isEnsureInitialized) {
      return;
    }
    try {
      _ggmlLibrarySharedBindingsByGeneralDeveloper =
          GgmlLibrarySharedBindingsByGeneralDeveloper(
        DynamicLibrary.open(
          libraryGgmlPath,
        ),
      );
      _isDeviceSupport = true;
    } catch (e) {
      print(e);
      _isCrash = true;
    }

    _isEnsureInitialized = true;
  }

  /// Check Out: https://www.youtube.com/@GENERAL_DEV
  bool _isCrash = false;

  /// Check Out: https://www.youtube.com/@GENERAL_DEV
  bool _isDeviceSupport = false;

  /// Check Out: https://www.youtube.com/@GENERAL_DEV
  @override
  bool isCrash() {
    return _isCrash;
  }

  /// Check Out: https://www.youtube.com/@GENERAL_DEV
  @override
  bool isDeviceSupport() {
    if (_isCrash) {
      return false;
    }
    return _isDeviceSupport;
  }

  /// Check Out: https://www.youtube.com/@GENERAL_DEV
  @override
  FutureOr<void> dispose() {}

  /// Check Out: https://www.youtube.com/@GENERAL_DEV
  GgmlLibrarySharedBindingsByGeneralDeveloper
      get ggmlLibrarySharedBindingsByGeneralDeveloper {
    return GgmlLibrary._ggmlLibrarySharedBindingsByGeneralDeveloper;
  }

  @override

  /// Check Out: https://www.youtube.com/@GENERAL_DEV
  FutureOr<void> initialized() async {}
}
