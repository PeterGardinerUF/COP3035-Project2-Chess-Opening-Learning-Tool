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

bool ContinuationsEquivalent(const Continuation& a, const Continuation& b) {
    return a.fromX == b.fromX && a.fromY == b.fromY && a.toX == b.toX && a.toY == b.toY && a.promotionPiece == b.promotionPiece && a.gameOver == b.gameOver;
}