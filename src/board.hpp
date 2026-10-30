#pragma once
#include <cstdint>
#include <string>
#include <vector>
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
    int fromX;
    int fromY;
    int toX;
    int toY;
    Piece promotionPiece;
    bool gameOver;
};

Continuation GetContinuationFromTokenUCI(const string& token);

void ApplyContinuation(Board& board, const Continuation continuation);

bool ContinuationsEquivalent(const Continuation& a, const Continuation& b);

struct BoardData {
    string description;
    uint32_t whiteWins;
    uint32_t blackWins;
    uint32_t draws;
    vector<Continuation> continuations;
};

Board InitialBoard();