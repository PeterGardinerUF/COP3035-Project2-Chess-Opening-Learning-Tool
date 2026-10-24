#pragma once
#include <cstdint>
#include <random>
#include <iostream>
#include "board.hpp"
using namespace std;

#define MAX_BOARD_COUNT 200000

struct ZobristKeys {
    uint64_t pieceSquare[UNIQUE_PIECE_COUNT][8][8];
    uint64_t whiteToPlay;
};

struct Bucket {
    bool occupied = false;
    uint64_t hashValue;
    uint32_t count;
    BoardData data;  
};

ZobristKeys InitializeZobristKeys();

class HashMap {
    
    uint64_t Hash(const Board& board) const;
    
    ZobristKeys keys;
    Bucket buckets[MAX_BOARD_COUNT];
    
    public:
    
    HashMap();
    
    void InsertBoard(const Board& board, BoardData data);
    
    BoardData Find(const Board& board) const;
    
};