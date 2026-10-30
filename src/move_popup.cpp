#include "move_popup.hpp"
#include "raylib.h"

static char label[8];
static char uci[5];
static int moveCount = 0;

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

    if (king && fromX == 4 && fromY == toY && (fromY == 0 || fromY == 7)) {
        if (toX == 6 || toX == 7) {
            label[0] = 'O'; label[1] = '-'; label[2] = 'O'; label[3] = 0;
            return;
        }
        if (toX == 2 || toX == 0) {
            label[0] = 'O'; label[1] = '-'; label[2] = 'O'; label[3] = '-'; label[4] = 'O'; label[5] = 0;
            return;
        }
    }

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
