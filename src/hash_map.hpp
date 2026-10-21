#pragma once
#include <cstdint>
#include <random>
#include <iostream>
#include "board.hpp"
using namespace std;

#define MAX_GAME_COUNT 200000

struct ZobristKeys {
    uint64_t pieceSquare[UNIQUE_PIECE_COUNT][8][8];
    uint64_t whiteToPlay;
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

struct Bucket {
    bool occupied = false;
    uint64_t hashValue;
    GameData data;  
    uint32_t count;
};

ZobristKeys InitializeZobristKeys();

class HashMap {
    
    uint64_t Hash(Board board) const;
    
    ZobristKeys keys;
    Bucket buckets[MAX_GAME_COUNT];
    
    public:
    
    HashMap();
    
    void Insert(Board board, GameData data);
    
};