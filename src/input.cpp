#include "input.hpp"
#include <algorithm>

Input InitialInput() {
    return (Input) {
        .state = DEFAULT,
        .endProgram = false,
        .selectedX = 0,
        .selectedY = 0,
        .toPlace = EMPTY,
    };
}

bool IsMouseOver(Button button, Vector2 mousePosition) {
    if (mousePosition.x < button.x || mousePosition.y < button.y || mousePosition.x > button.x + button.width || mousePosition.y > button.y + button.height) {
        return false;
    }
    return true;
}

void HandleInput(Board& game, Input& input, int boardSizePixels) {
    
    Button boardButton = {
        .x = 0,
        .y = 0,
        .width = boardSizePixels,
        .height = boardSizePixels,
    };
    
    Button pieceBar = {
        .x = boardSizePixels,
        .y = 0,
        .width = boardSizePixels,
        .height = boardSizePixels / 16,
    };
    
    Vector2 mousePosition = GetMousePosition();
    int clickedX = (8 * mousePosition.x) / boardSizePixels;
    int clickedY = 8 - (8 * mousePosition.y) / boardSizePixels;
    clickedX = max(min(clickedX, 8), 0);
    clickedY = max(min(clickedY, 8), 0);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (IsMouseOver(boardButton, mousePosition)) {
            if (input.state == DEFAULT) {
                if (game.pieces[clickedX][clickedY] != EMPTY) {
                    input.selectedX = clickedX;
                    input.selectedY = clickedY;
                    input.state = PIECE_SELECTED;
                }
            } else if (input.state == PIECE_SELECTED) {
                Piece piece = game.pieces[input.selectedX][input.selectedY];
                game.pieces[input.selectedX][input.selectedY] = EMPTY;
                game.pieces[clickedX][clickedY] = piece;
                input.state = DEFAULT;
            } else if (input.state == PLACE_MODE) {
                game.pieces[clickedX][clickedY] = input.toPlace;
            }
        } else if (IsMouseOver(pieceBar, mousePosition)) {
            Piece toPlace = (Piece)((mousePosition.x - boardSizePixels) * 16 / boardSizePixels);
            if (input.toPlace == toPlace) {
                input.toPlace = EMPTY;
                input.state = DEFAULT;
            } else {
                input.state = PLACE_MODE;
                input.toPlace = toPlace;
            }
        }
    } else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        game.pieces[clickedX][clickedY] = EMPTY;
        if (input.state == PIECE_SELECTED && clickedX == input.selectedX && clickedY == input.selectedY) {
            input.state = DEFAULT;
        }
    }
    
    if (input.state == PIECE_SELECTED && (IsKeyDown(KEY_DELETE) || IsKeyDown(KEY_BACKSPACE))) {
        game.pieces[input.selectedX][input.selectedY] = EMPTY;
        input.state = DEFAULT;
    }
    
    if (IsKeyDown(KEY_ESCAPE)) {
        input.state = DEFAULT;
        input.toPlace = EMPTY;
    }
}