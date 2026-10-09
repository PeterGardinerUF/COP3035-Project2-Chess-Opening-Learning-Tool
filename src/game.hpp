#pragma once
#include <cstdint>
#include <iostream>
using namespace std;

enum PieceType : uint64_t {
    WHITE_PAWN   = UINT64_C(0x616179973C9501D7),
    WHITE_KNIGHT = UINT64_C(0xB8C9A9E0EEAE0704),
    WHITE_BISHOP = UINT64_C(0x8210A92080DA388A),
    WHITE_ROOK   = UINT64_C(0x100B145D0AD33531),
    WHITE_QUEEN  = UINT64_C(0x94E2F97C6834F301),
    WHITE_KING   = UINT64_C(0xC196E4AB04A8FAE3),
    BLACK_PAWN   = UINT64_C(0x78463B01BA076DD8),
    BLACK_KNIGHT = UINT64_C(0xE6AF5B7111A257F4),
    BLACK_BISHOP = UINT64_C(0x5211DBCEEA847934),
    BLACK_ROOK   = UINT64_C(0xED655BF933311C30),
    BLACK_QUEEN  = UINT64_C(0xF392F2E685CD45D9),
    BLACK_KING   = UINT64_C(0xE7C0920E345A306B),
};

typedef struct {
    PieceType type;
    int positionIndex;
    bool alive;
} Piece;

typedef struct {
    Piece pieces[32];
    bool whiteToPlay;
} Game;

int CoordinateToPosition(int x, int y);

Piece MakePiece();

Game InitialGame();

const char* pieceSymbol(PieceType type);