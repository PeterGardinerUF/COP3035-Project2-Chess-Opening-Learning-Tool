#pragma once
#include <cstdint>

#define UNIQUE_PIECE_COUNT 12

enum Piece {
    WHITE_PAWN,
    WHITE_KNIGHT,
    WHITE_BISHOP,
    WHITE_ROOK,
    WHITE_QUEEN,
    WHITE_KING,
    BLACK_PAWN,
    BLACK_KNIGHT,
    BLACK_BISHOP,
    BLACK_ROOK,
    BLACK_QUEEN,
    BLACK_KING,
    EMPTY,
};

struct Board {
    Piece pieces[8][8];
    bool whiteToPlay;
};

Board InitialBoard();