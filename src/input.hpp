#pragma once
#include <algorithm>
#include "raylib.h"
#include "board.hpp"
using namespace std;

enum InputState {
    DEFAULT,
    PIECE_SELECTED,
    PLACE_MODE,
};

typedef struct {
    InputState state;
    bool endProgram;
    bool boardModified;
    int selectedX;
    int selectedY;
    Piece toPlace;
} Input;

typedef struct {
    int x;
    int y;
    int width;
    int height;
} Button;

Input InitialInput();

bool IsMouseOver(Button button, Vector2 mousePosition);

void HandleInput(Board& game, Input& input, int boardSizePixels);