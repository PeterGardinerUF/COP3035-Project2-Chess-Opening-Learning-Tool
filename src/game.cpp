#include "game.hpp"

int CoordinateToPosition(int x, int y) {
    if (x < 1 || x > 8 || y < 1 || y > 8) {
        return -1;
    }
    
    return 8 * (8 - y) + x - 1;
}

Piece MakePiece(const PieceType type, int x, int y) {
    Piece piece;
    piece.type = type;
    piece.positionIndex = CoordinateToPosition(x, y);
    piece.alive = true;
    return piece;
}

Game InitialGame() {
    Game game;
    game.whiteToPlay = true;
    for (int i = 1; i <= 8; i++) {
        game.pieces[i - 1] = MakePiece(WHITE_PAWN, i, 2);
        game.pieces[i + 7] = MakePiece(BLACK_PAWN, i, 7);
    }
    int i = 16;
    game.pieces[i] = MakePiece(WHITE_ROOK, 1, 1); i++;
    game.pieces[i] = MakePiece(WHITE_ROOK, 8, 1); i++;
    game.pieces[i] = MakePiece(BLACK_ROOK, 1, 8); i++;
    game.pieces[i] = MakePiece(BLACK_ROOK, 8, 8); i++;
    
    game.pieces[i] = MakePiece(WHITE_KNIGHT, 2, 1); i++;
    game.pieces[i] = MakePiece(WHITE_KNIGHT, 7, 1); i++;
    game.pieces[i] = MakePiece(BLACK_KNIGHT, 2, 8); i++;
    game.pieces[i] = MakePiece(BLACK_KNIGHT, 7, 8); i++;
    
    game.pieces[i] = MakePiece(WHITE_BISHOP, 3, 1); i++;
    game.pieces[i] = MakePiece(WHITE_BISHOP, 6, 1); i++;
    game.pieces[i] = MakePiece(BLACK_BISHOP, 3, 8); i++;
    game.pieces[i] = MakePiece(BLACK_BISHOP, 6, 8); i++;
    
    game.pieces[i] = MakePiece(WHITE_QUEEN, 4, 1); i++;
    game.pieces[i] = MakePiece(BLACK_QUEEN, 4, 8); i++;
    
    game.pieces[i] = MakePiece(WHITE_KING, 5, 1); i++;
    game.pieces[i] = MakePiece(BLACK_KING, 5, 8); i++;
    
    return game;
}

const char* pieceSymbol(PieceType type) {
    switch(type) {
        case WHITE_PAWN:   return "wp";
        case WHITE_KNIGHT: return "wn";
        case WHITE_BISHOP: return "wb";
        case WHITE_ROOK:   return "wr";
        case WHITE_QUEEN:  return "wq";
        case WHITE_KING:   return "wk";
        case BLACK_PAWN:   return "bp";
        case BLACK_KNIGHT: return "bn";
        case BLACK_BISHOP: return "bb";
        case BLACK_ROOK:   return "br";
        case BLACK_QUEEN:  return "bq";
        case BLACK_KING:   return "bk";
    }
    
    return "??";
}