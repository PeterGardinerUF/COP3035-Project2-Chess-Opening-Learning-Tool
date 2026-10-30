#include "move_popup.hpp"
#include "raylib.h"

static char label[8];
static char uci[5];
static int moveCount = 0;
static int castleWait = 0;

static char PieceLetter(Piece piece) {
    if (piece == WHITE_KNIGHT || piece == BLACK_KNIGHT) return 'N';
    if (piece == WHITE_BISHOP || piece == BLACK_BISHOP) return 'B';
    if (piece == WHITE_ROOK || piece == BLACK_ROOK) return 'R';
    if (piece == WHITE_QUEEN || piece == BLACK_QUEEN) return 'Q';
    if (piece == WHITE_KING || piece == BLACK_KING) return 'K';
    return 0;
}

void NoteMove(int fromX, int fromY, int toX, int toY, Piece piece, Piece captured) {
    moveCount++;
    int endX = toX;
    bool king = piece == WHITE_KING || piece == BLACK_KING;
    if (king && fromX == 4 && fromY == toY && (fromY == 0 || fromY == 7)) {
        if (toX == 6) endX = 7;
        if (toX == 2) endX = 0;
    }
    uci[0] = 'a' + fromX;
    uci[1] = '1' + fromY;
    uci[2] = 'a' + endX;
    uci[3] = '1' + toY;
    uci[4] = 0;

    if (castleWait == 1 && piece == WHITE_ROOK && fromX == 7 && fromY == 0 && toX == 5 && toY == 0) {
        castleWait = 0;
        label[0] = 'O'; label[1] = '-'; label[2] = 'O'; label[3] = 0;
        return;
    }
    if (castleWait == 2 && piece == WHITE_ROOK && fromX == 0 && fromY == 0 && toX == 3 && toY == 0) {
        castleWait = 0;
        label[0] = 'O'; label[1] = '-'; label[2] = 'O'; label[3] = '-'; label[4] = 'O'; label[5] = 0;
        return;
    }
    if (castleWait == 3 && piece == BLACK_ROOK && fromX == 7 && fromY == 7 && toX == 5 && toY == 7) {
        castleWait = 0;
        label[0] = 'O'; label[1] = '-'; label[2] = 'O'; label[3] = 0;
        return;
    }
    if (castleWait == 4 && piece == BLACK_ROOK && fromX == 0 && fromY == 7 && toX == 3 && toY == 7) {
        castleWait = 0;
        label[0] = 'O'; label[1] = '-'; label[2] = 'O'; label[3] = '-'; label[4] = 'O'; label[5] = 0;
        return;
    }

    castleWait = 0;
    if (king && fromX == 4 && fromY == 0 && toY == 0 && toX == 6) castleWait = 1;
    if (king && fromX == 4 && fromY == 0 && toY == 0 && toX == 2) castleWait = 2;
    if (king && fromX == 4 && fromY == 7 && toY == 7 && toX == 6) castleWait = 3;
    if (king && fromX == 4 && fromY == 7 && toY == 7 && toX == 2) castleWait = 4;

    int n = 0;
    char letter = PieceLetter(piece);
    bool capture = captured != EMPTY;
    if (letter) label[n++] = letter;
    else if (capture) label[n++] = 'a' + fromX;
    if (capture) label[n++] = 'x';
    label[n++] = 'a' + toX;
    label[n++] = '1' + toY;
    label[n] = 0;
}

const char* LastUci() {
    return uci;
}

int MoveCount() {
    return moveCount;
}

void DrawLastMove() {
    if (!label[0]) return;
    DrawRectangle(704, 96, 150, 44, WHITE);
    DrawText(label, 720, 104, 32, BLACK);
}
