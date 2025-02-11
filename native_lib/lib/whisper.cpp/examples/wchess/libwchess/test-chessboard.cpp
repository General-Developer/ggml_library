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
#include "Chessboard.h"

#define ASSERT(x) \
    do { \
        if (!(x)) { \
            fprintf(stderr, "ASSERT: %s:%d: %s\n", __FILE__, __LINE__, #x); \
            fflush(stderr); \
            exit(1); \
        } \
    } while (0)


int main() {
    {
        Chessboard chess;

        ASSERT(chess.process("pawn to d4") == "d2-d4");
        ASSERT(chess.process("e5") == "e7-e5");
        ASSERT(chess.process("c1 h6") == "c1-h6");
        ASSERT(chess.process("queen h4") == "d8-h4");
        ASSERT(chess.process("bishop to g5") == "h6-g5");
        ASSERT(chess.process("bishop to b4") == "f8-b4");
        ASSERT(chess.process("c4") == "");
        ASSERT(chess.process("knight c3") == "b1-c3");
        ASSERT(chess.process("knight c6") == "b8-c6");
        ASSERT(chess.process("f3") == "");
    }

    {
        Chessboard chess;

        ASSERT(chess.process("d4") == "d2-d4");
        ASSERT(chess.process("e5") == "e7-e5");
        ASSERT(chess.process("e4") == "e2-e4");
        ASSERT(chess.process("queen h4") == "d8-h4");
        ASSERT(chess.process("queen h5") == "d1-h5");
        ASSERT(chess.process("f5") == "");
        ASSERT(chess.process("g6") == "g7-g6");
        ASSERT(chess.process("knight e2") == "g1-e2");
        ASSERT(chess.process("f5") == "f7-f5");
        ASSERT(chess.process("knight g3") == "e2-g3");
        ASSERT(chess.process("g5") == "");
        ASSERT(chess.process("king e7") == "e8-e7");
        ASSERT(chess.process("f4") == "f2-f4");
        ASSERT(chess.process("g5") == "g6-g5");
    }

    {
        Chessboard chess;

        ASSERT(chess.process("e4") == "e2-e4");
        ASSERT(chess.process("c5") == "c7-c5");
        ASSERT(chess.process("e5") == "e4-e5");
        ASSERT(chess.process("c4") == "c5-c4");
        ASSERT(chess.process("e6") == "e5-e6");
        ASSERT(chess.process("c3") == "c4-c3");
        ASSERT(chess.process("e7") == "");
        ASSERT(chess.process("f7") == "e6-f7");
        ASSERT(chess.process("d2") == "");
        ASSERT(chess.process("king to f7") == "e8-f7");
        ASSERT(chess.process("f4") == "f2-f4");
        ASSERT(chess.process("d2") == "c3-d2");
        ASSERT(chess.process("f5") == "");
        ASSERT(chess.process("king to e2") == "e1-e2");
        ASSERT(chess.process("king to g6") == "f7-g6");
        ASSERT(chess.process("f5") == "f4-f5");
        ASSERT(chess.process("e6") == "");
        ASSERT(chess.process("king to h5") == "g6-h5");
        ASSERT(chess.process("g4") == "g2-g4");
        ASSERT(chess.process("king to g5") == "h5-g5");
        ASSERT(chess.process("h4") == "h2-h4");
        ASSERT(chess.process("king to h5") == "");
        ASSERT(chess.process("king to g6") == "");
        ASSERT(chess.process("king to h6") == "g5-h6");
        ASSERT(chess.process("bishop to d2") == "c1-d2");
        ASSERT(chess.process("king to g5") == "");
        ASSERT(chess.process("g5") == "g7-g5");
    }

    {
        Chessboard chess;
        ASSERT(chess.process("f4") == "f2-f4");
        ASSERT(chess.process("e5") == "e7-e5");
        ASSERT(chess.process("g4") == "g2-g4");
        ASSERT(chess.process("queen to h4") == "d8-h4#");
        ASSERT(chess.process("knight f3") == "");
        ASSERT(chess.grammar().empty());
    }

    {
        Chessboard chess;
        ASSERT(chess.process("f4") == "f2-f4");
        ASSERT(chess.process("e5") == "e7-e5");
        ASSERT(chess.process("g4") == "g2-g4");
        ASSERT(chess.process("d5") == "d7-d5");
        ASSERT(chess.process("g1 f3") == "g1-f3");
        ASSERT(chess.process("queen to h4") == "d8-h4");
        ASSERT(!chess.grammar().empty());
    }

    {
        Chessboard chess;
        ASSERT(chess.process("knight c3") == "b1-c3");
        ASSERT(chess.process("knight c6") == "b8-c6");
        ASSERT(chess.process("knight b5") == "c3-b5");
        ASSERT(chess.process("knight f6") == "g8-f6");
        ASSERT(chess.process("knight d6") == "b5-d6");
        ASSERT(chess.process("knight d4") == "");
        ASSERT(chess.process("d6") == "c7-d6");
        ASSERT(chess.process("e4") == "e2-e4");
        ASSERT(chess.process("knight d4") == "c6-d4");
        ASSERT(chess.process("d3") == "d2-d3");
        ASSERT(chess.process("knight e4") == "f6-e4");
        ASSERT(chess.process("king to e2") == "");
        ASSERT(chess.process("king to d2") == "");
    }
}