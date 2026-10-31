#include "board.hpp"

Board InitialBoard() {
    Board board;
    board.whiteToPlay = true;
    
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            board.pieces[x][y] = EMPTY;
        }
    }
    
    Piece blackRow[] = {
        BLACK_ROOK, BLACK_KNIGHT, BLACK_BISHOP, BLACK_QUEEN,
        BLACK_KING, BLACK_BISHOP, BLACK_KNIGHT, BLACK_ROOK
    };
    Piece whiteRow[] = {
        WHITE_ROOK, WHITE_KNIGHT, WHITE_BISHOP, WHITE_QUEEN,
        WHITE_KING, WHITE_BISHOP, WHITE_KNIGHT, WHITE_ROOK
    };
    for (int x = 0; x < 8; x++) {
        board.pieces[x][7] = blackRow[x];
        board.pieces[x][6] = BLACK_PAWN;
        board.pieces[x][1] = WHITE_PAWN;
        board.pieces[x][0] = whiteRow[x];
    }

    return board;
}

Continuation GetContinuationFromTokenUCI(const string& token) {
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

void ApplyContinuation(Board& board, const Continuation continuation) {
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

bool ContinuationsEquivalent(const Continuation& a, const Continuation& b) {
    return a.fromX == b.fromX && a.fromY == b.fromY && a.toX == b.toX && a.toY == b.toY && a.promotionPiece == b.promotionPiece && a.gameOver == b.gameOver;
}

bool ContinuationGreaterThan(const Continuation& a, const Continuation& b) {
    return a.count > b.count;
}

bool ContinuationLessThan(const Continuation& a, const Continuation& b) {
    return a.count < b.count;
}