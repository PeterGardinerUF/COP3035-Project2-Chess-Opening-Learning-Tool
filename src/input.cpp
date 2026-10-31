#include "input.hpp"
#include "move_popup.hpp"

Input InitialInput() {
    return (Input) {
        .state = DEFAULT,
        .endProgram = false,
        .boardModified = true,
        .usingTrie = true,
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

void HandleInput(Board& board, Input& input, int boardSizePixels) {
    input.boardModified = false;
    
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
    
    Button colorToPlayButton = {
        .x = boardSizePixels,
        .y = boardSizePixels / 16,
        .width = 300,
        .height = boardSizePixels / 16,
    };
    
    Button dataStructureToggle = {
        .x = boardSizePixels,
        .y = boardSizePixels / 16 * 4,
        .width = 300,
        .height = boardSizePixels / 16,
    };
    
    Vector2 mousePosition = GetMousePosition();
    int clickedX = (8 * mousePosition.x) / boardSizePixels;
    int clickedY = 8 - (8 * mousePosition.y) / boardSizePixels;
    clickedX = max(min(clickedX, 7), 0);
    clickedY = max(min(clickedY, 7), 0);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (IsMouseOver(boardButton, mousePosition)) {
            if (input.state == DEFAULT) {
                if (board.pieces[clickedX][clickedY] != EMPTY) {
                    input.selectedX = clickedX;
                    input.selectedY = clickedY;
                    input.state = PIECE_SELECTED;
                }
            } else if (input.state == PIECE_SELECTED) {
                if (!(input.selectedX == clickedX && input.selectedY == clickedY)) {
                    Piece piece = board.pieces[input.selectedX][input.selectedY];
                    Piece captured = board.pieces[clickedX][clickedY];
                    NoteMove(input.selectedX, input.selectedY, clickedX, clickedY, piece, captured);
                    board.pieces[input.selectedX][input.selectedY] = EMPTY;
                    board.pieces[clickedX][clickedY] = piece;
                    board.whiteToPlay = !board.whiteToPlay;
                    input.boardModified = true;
                }
                input.state = DEFAULT;
            } else if (input.state == PLACE_MODE) {
                board.pieces[clickedX][clickedY] = input.toPlace;
                input.boardModified = true;
            }
        } else if (IsMouseOver(pieceBar, mousePosition)) {
            Piece toPlace = (Piece)((mousePosition.x - boardSizePixels) * 16 / boardSizePixels);
            if (toPlace < EMPTY) {
                if (input.toPlace == toPlace) {
                    input.toPlace = EMPTY;
                    input.state = DEFAULT;
                } else {
                    input.state = PLACE_MODE;
                    input.toPlace = toPlace;
                }
            }
        } else if (IsMouseOver(colorToPlayButton, mousePosition)) {
            board.whiteToPlay = !board.whiteToPlay;
            input.boardModified = true;
        } else if (IsMouseOver(dataStructureToggle, mousePosition)) {
            input.boardModified = true;
            input.usingTrie = !input.usingTrie;
            if (input.usingTrie) {
                board = InitialBoard();
            }
        }
    } else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        if (IsMouseOver(boardButton, mousePosition)) {
            board.pieces[clickedX][clickedY] = EMPTY;
            if (input.state == PIECE_SELECTED && clickedX == input.selectedX && clickedY == input.selectedY) {
                input.state = DEFAULT;
            }
            input.boardModified = true;
        }
    }
    
    if (input.state == PIECE_SELECTED && (IsKeyDown(KEY_DELETE) || IsKeyDown(KEY_BACKSPACE))) {
        board.pieces[input.selectedX][input.selectedY] = EMPTY;
        input.state = DEFAULT;
        input.boardModified = true;
    }
    
    if (IsKeyDown(KEY_ESCAPE)) {
        input.state = DEFAULT;
        input.toPlace = EMPTY;
    }
}