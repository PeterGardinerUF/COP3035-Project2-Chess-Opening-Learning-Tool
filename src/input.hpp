#pragma once
#include "raylib.h"
#include "game.hpp"
#include "renderer.hpp"

enum InputState {
    DEFAULT,
    PIECE_SELECTED,
};

typedef struct {
    InputState state;
    bool endProgram;
    int selectedX;
    int selectedY;
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