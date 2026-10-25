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
    buckets = (Bucket*)malloc(MAX_BOARD_COUNT * sizeof(Bucket));
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

Continuation HashMap::GetContinuationFromToken(const string& token) const {
    Continuation continuation;
    if (token.length() < 4) {
        continuation.gameOver = true;
        return continuation;
    }
    continuation.gameOver = false;
    continuation.fromX = token[0] - 'a';
    continuation.fromY = token[1] - '0' - 1;
    continuation.toX = token[2] - 'a';
    continuation.toY = token[3] - '0' - 1;
    continuation.promotionPiece = EMPTY;
    
    if (continuation.toY == 0 || continuation.toY == 7) {
        char promotionChar = token[4];
        int promotionPiece;
        if (promotionChar == 'q') {
            promotionPiece = WHITE_QUEEN;
        } else if (promotionChar == 'r') {
            promotionPiece = WHITE_ROOK;
        } else if (promotionChar == 'b') {
            promotionPiece = WHITE_BISHOP;
        } else if (promotionChar == 'n') {
            promotionPiece = WHITE_KNIGHT;
        } else {
            promotionPiece = EMPTY;
        }
        if (continuation.toY == 0) {
            promotionPiece += (BLACK_QUEEN - WHITE_QUEEN);
        }
        continuation.promotionPiece = (Piece)promotionPiece;
    }
    return continuation;
}

void HashMap::ApplyContinuation(Board& board, const Continuation continuation) const {
    if (continuation.gameOver) {
        return;
    }
    int fromX = continuation.fromX;
    int fromY = continuation.fromY;
    int toX = continuation.toX;
    int toY = continuation.toY;
    
    if (fromX == 4 && fromY == 0 && board.pieces[4][0] == WHITE_KING && toY == 0) {
        if (toX == 0) {
            board.pieces[0][0] = EMPTY;
            board.pieces[2][0] = WHITE_KING;
            board.pieces[3][0] = WHITE_ROOK;
            board.pieces[3][0] = EMPTY;
        } else if (toX == 7) {
            board.pieces[4][0] = EMPTY;
            board.pieces[5][0] = WHITE_ROOK;
            board.pieces[6][0] = WHITE_KING;
            board.pieces[7][0] = EMPTY;
        }
        board.whiteToPlay = !board.whiteToPlay;
        return;
    } else if (fromX == 4 && fromY == 7 && board.pieces[4][7] == BLACK_KING && toY == 7) {
        if (toX == 0) {
            board.pieces[0][7] = EMPTY;
            board.pieces[2][7] = BLACK_KING;
            board.pieces[3][7] = BLACK_ROOK;
            board.pieces[3][7] = EMPTY;
        } else if (toX == 7) {
            board.pieces[4][7] = EMPTY;
            board.pieces[5][7] = BLACK_ROOK;
            board.pieces[6][7] = BLACK_KING;
            board.pieces[7][7] = EMPTY;
        }
        board.whiteToPlay = !board.whiteToPlay;
        return;
    }
     
    if (board.pieces[fromX][fromY] == WHITE_PAWN || board.pieces[fromX][fromY] == BLACK_PAWN) {
        if (board.pieces[toX][toY] == EMPTY && fromX != toX) { // En passant
            board.pieces[toX][toY] = board.pieces[fromX][fromY];
            board.pieces[fromX][fromY] = EMPTY;
            board.pieces[toX][fromY] = EMPTY;
            board.whiteToPlay = !board.whiteToPlay;
            return;
        } else if (toY == 0 || toY == 7) { // Promotion
            board.pieces[fromX][fromY] = EMPTY;
            board.pieces[toX][toY] = (Piece)continuation.promotionPiece;
            board.whiteToPlay = !board.whiteToPlay;
            return;
        }
    }
    board.pieces[toX][toY] = board.pieces[fromX][fromY];
    board.pieces[fromX][fromY] = EMPTY;
    board.whiteToPlay = !board.whiteToPlay;
}

void HashMap::InsertUCI(string uci) {
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
        Continuation advanceBoardContinuation = GetContinuationFromToken(token);
        ApplyContinuation(board, advanceBoardContinuation);
        
        string continuationToken;
        streampos pos = stringStream.tellg();
        stringStream >> continuationToken;
        stringStream.seekg(pos);
        Continuation continuation = GetContinuationFromToken(continuationToken);
        InsertBoard(board, result, continuation);
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
            buckets[index].data.continuations.push_back(continuation); // TODO: Reconsider continuations
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
            buckets[index].data.continuations.push_back(continuation); // TODO: Reconsider continuations
            break;
        }
    }
}

bool HashMap::Find(const Board& board, BoardData* inData) const {
    const uint64_t h = Hash(board);
    for (uint32_t i = 0; i < MAX_BOARD_COUNT; i++) {
        uint32_t index = (h + i) % MAX_BOARD_COUNT;
        if (buckets[index].hashValue == h) {
            *inData = buckets[index].data;
            return true;
        }
    }
    return false;
}