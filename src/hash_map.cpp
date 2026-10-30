#include "hash_map.hpp"
#include "board.hpp"

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
    buckets = (Bucket*)malloc(MAX_BOARD_COUNT * sizeof(Bucket));
    uniqueCount = 0;
    totalCount = 0;
}

HashMap::~HashMap() {
    free(buckets);
}

uint64_t HashMap::Hash(const Board& board) const {
    uint64_t hash = 0;
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            Piece piece = board.pieces[x][y];
            if (piece >= EMPTY) continue;
            hash ^= keys.pieceSquare[piece][x][y];
        }
    }
    if (board.whiteToPlay) {
        hash ^= keys.whiteToPlay;
    }
    
    return hash;
}

float HashMap::loadFactor() const {
    return (float)uniqueCount / MAX_BOARD_COUNT;
}

void HashMap::InsertFileUCI(const string& filePath) {
    ifstream input(filePath);
    if (!input) {
        return;
    }
    
    string line;
    while (getline(input, line)) {
        InsertUCI(line);
    }
}

void HashMap::InsertUCI(const string& uci) {
    Board board = InitialBoard();
    int result;
    
    istringstream stringStream(uci);
    string gameResult;
    stringStream >> gameResult;
    if (gameResult == "1-0") {
        result = 1;
    } else if (gameResult == "0-1") {
        result = -1;
    } else if (gameResult == "1/2-1/2") {
        result = 0;
    }
    
    string token;
    while (stringStream >> token) {
        string continuationToken;
        streampos pos = stringStream.tellg();
        stringStream >> continuationToken;
        stringStream.seekg(pos);
        Continuation continuation = GetContinuationFromTokenUCI(continuationToken);
        InsertBoard(board, result, continuation);
        
        Continuation advanceBoardContinuation = GetContinuationFromTokenUCI(token);
        ApplyContinuation(board, advanceBoardContinuation);
    }
}

void HashMap::InsertBoard(const Board& board, int result, Continuation continuation) {
    const uint64_t h = Hash(board);
    
    for (uint32_t i = 0; i < MAX_BOARD_COUNT; i++) {
        uint32_t index = (h + i) % MAX_BOARD_COUNT;
        if (!buckets[index].occupied) {
            buckets[index].hashValue = h;
            buckets[index].occupied = true;
            buckets[index].count = 1;
            if (result == 1) {
                buckets[index].data.whiteWins++;
            } else if (result == -1) {
                buckets[index].data.blackWins++;
            } else if (result == 0) {
                buckets[index].data.draws++;
            }
            
            bool continuationFound = false;
            for (Continuation& cont : buckets[index].data.continuations) {
                if (ContinuationsEquivalent(cont, continuation)) {
                    cont.count++;
                    continuationFound = true;
                    break;
                }
            }
            
            if (!continuationFound) {
                buckets[index].data.continuations.push_back(continuation);
            }
            
            sort(buckets[index].data.continuations.begin(), buckets[index].data.continuations.end(), ContinuationGreaterThan);
            
            totalCount++;
            uniqueCount++;
            break;
        } else if (buckets[index].hashValue == h) {
            buckets[index].count++;
            if (result == 1) {
                buckets[index].data.whiteWins++;
            } else if (result == -1) {
                buckets[index].data.blackWins++;
            } else if (result == 0) {
                buckets[index].data.draws++;
            }
            
            bool continuationFound = false;
            for (Continuation& cont : buckets[index].data.continuations) {
                if (ContinuationsEquivalent(cont, continuation)) {
                    cont.count++;
                    continuationFound = true;
                    break;
                }
            }
            
            if (!continuationFound) {
                buckets[index].data.continuations.push_back(continuation);
            }
            
            sort(buckets[index].data.continuations.begin(), buckets[index].data.continuations.end(), ContinuationGreaterThan);
            
            totalCount++;
            break;
        }
    }
}

bool HashMap::Find(const Board& board, BoardData* inData) const {
    const uint64_t h = Hash(board);
    for (uint32_t i = 0; i < MAX_BOARD_COUNT; i++) {
        uint32_t index = (h + i) % MAX_BOARD_COUNT;
        if (!buckets[index].occupied) {
            return false;
        }
        if (buckets[index].hashValue == h) {
            *inData = buckets[index].data;
            return true;
        }
    }
    return false;
}