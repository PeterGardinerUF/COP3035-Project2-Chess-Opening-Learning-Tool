#pragma once
#include "board.hpp"

void NoteMove(int fromX, int fromY, int toX, int toY, Piece piece, Piece captured);
void DrawLastMove();
const char* LastUci();
int MoveCount();
