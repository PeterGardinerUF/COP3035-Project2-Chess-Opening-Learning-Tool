#pragma once
#include <cstdint>
#include <random>
#include <sstream>
#include <fstream>
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
    
    ZobristKeys keys;
    Bucket* buckets;
    uint32_t uniqueCount;
    uint32_t totalCount;
    
    float loadFactor() const;
    
    public:
    
    uint64_t Hash(const Board& board) const;
    
    HashMap();
    ~HashMap();
    
    void InsertFileUCI(const string& filePath);
    
    void InsertUCI(const string& uci);
    
    void InsertBoard(const Board& board, int result, Continuation continuation);
    
    bool Find(const Board& board, BoardData* inData) const;
    
};