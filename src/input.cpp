#include "input.hpp"

Input InitialInput() {
    return (Input) {
        .state = DEFAULT,
        .endProgram = false,
        .selectedX = -1,
        .selectedY = -1,
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
    
    Vector2 mousePosition = GetMousePosition();
    int clickedX = (8 * mousePosition.x) / boardSizePixels;
    int clickedY = 8 - (8 * mousePosition.y) / boardSizePixels;
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
            }
        }
    } else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        game.pieces[clickedX][clickedY] = EMPTY;
        if (clickedX == input.selectedX && clickedY == input.selectedY) {
            input.state = DEFAULT;
        }
    }
    
    if (IsKeyDown(KEY_DELETE) || IsKeyDown(KEY_BACKSPACE)) {
        game.pieces[input.selectedX][input.selectedY] = EMPTY;
        input.state = DEFAULT;
    }
}