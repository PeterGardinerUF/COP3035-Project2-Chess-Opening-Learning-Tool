#pragma once
#include <cstdint>
#include <string>
using namespace std;

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

struct SquareDelta {
    int x, y;
    Piece piece;
};

struct Continuation {
    uint32_t count;
    int squareDeltaCount;
    SquareDelta deltas[3];
};

struct GameData {
    float whiteElo;
    float blackElo;
    string description;
    Continuation continuations[3];
};

Board InitialBoard();