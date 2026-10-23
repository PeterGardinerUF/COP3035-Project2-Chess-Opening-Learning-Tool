#include "hash_map.hpp"

ZobristKeys InitializeZobristKeys() {
    ZobristKeys keys;
    
    mt19937_64 rng(0);
    uniform_int_distribution<uint64_t> dist;
    
    for (int i = 0; i < UNIQUE_PIECE_COUNT; i++) {
        for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++) {
                keys.pieceSquare[i][x][y] = dist(rng);
            }
        }
    }
    
    keys.whiteToPlay = dist(rng);
    
    return keys;
}

HashMap::HashMap() {
    keys = InitializeZobristKeys();
}

uint64_t HashMap::Hash(Board board) const {
    uint64_t hash;
    for (int x = 0; x < 64; x++) {
        for (int y = 0; y < 64; y++) {
            hash ^= keys.pieceSquare[board.pieces[x][y]][x][y];
        }
    }
    hash ^= board.whiteToPlay;
    
    return hash;
}

void HashMap::Insert(Board board, GameData data) {
    uint64_t h = Hash(board);
    
    for (uint32_t i = 0; i < MAX_GAME_COUNT; i++) {
        uint32_t index = (h + i) % MAX_GAME_COUNT;
        if (!buckets[index].occupied) {
            buckets[index].hashValue = h;
            buckets[index].occupied = true;
            buckets[index].data = data;
            buckets[index].count = 1;
            // TODO: Reconsider continuations
            break;
        } else if (buckets[index].hashValue == h) {
            buckets[index].count++;
            buckets[index].data.accumulatedWhiteElo += data.accumulatedWhiteElo;
            buckets[index].data.accumulatedBlackElo += data.accumulatedBlackElo;
            // TODO: Reconsider continuations
            break;
        }
    }
}