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

struct Bucket {
    bool occupied = false;
    uint64_t hashValue;
    uint32_t count;
    GameData data;  
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