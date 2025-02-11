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
# Add new build types

# ReleaseGG - Release with enabled asserts

SET(CMAKE_CXX_FLAGS_RELEASEGG
    "-O3"
    CACHE STRING "Flags used by the c++ compiler during release builds with enabled asserts."
    FORCE )
SET(CMAKE_C_FLAGS_RELEASEGG
    "-O3"
    CACHE STRING "Flags used by the compiler during release builds with enabled asserts."
    FORCE )
SET(CMAKE_EXE_LINKER_FLAGS_RELEASEGG
    ""
    CACHE STRING "Flags used for linking binaries during release builds with enabled asserts."
    FORCE )
SET(CMAKE_SHARED_LINKER_FLAGS_RELEASEGG
    ""
    CACHE STRING "Flags used by the shared libraries linker during release builds with enabled asserts."
    FORCE )
MARK_AS_ADVANCED(
    CMAKE_CXX_FLAGS_RELEASEGG
    CMAKE_C_FLAGS_RELEASEGG
    CMAKE_EXE_LINKER_FLAGS_RELEASEGG
    CMAKE_SHARED_LINKER_FLAGS_RELEASEGG )

# RelWithDebInfoGG - RelWithDebInfo with enabled asserts

SET(CMAKE_CXX_FLAGS_RELWITHDEBINFOGG
    "-O2 -g"
    CACHE STRING "Flags used by the c++ compiler during release builds with debug symbols and enabled asserts."
    FORCE )
SET(CMAKE_C_FLAGS_RELWITHDEBINFOGG
    "-O2 -g"
    CACHE STRING "Flags used by the compiler during release builds with debug symbols and enabled asserts."
    FORCE )
SET(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFOGG
    ""
    CACHE STRING "Flags used for linking binaries during release builds with debug symbols and enabled asserts."
    FORCE )
SET(CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFOGG
    ""
    CACHE STRING "Flags used by the shared libraries linker during release builds with debug symbols and enabled asserts."
    FORCE )
MARK_AS_ADVANCED(
    CMAKE_CXX_FLAGS_RELWITHDEBINFOGG
    CMAKE_C_FLAGS_RELWITHDEBINFOGG
    CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFOGG
    CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFOGG )

if (NOT XCODE AND NOT MSVC AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo" "ReleaseGG" "RelWithDebInfoGG")
endif()
