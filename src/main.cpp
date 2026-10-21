#include "board.hpp"
#include "renderer.hpp"
#include "input.hpp"
#include "hash_map.hpp"
// g++ src/*.cpp -o build/debug.exe -Wall -Wextra -g -IC:/raylib/raylib/src -LC:/raylib/raylib/src -lraylib -lopengl32 -lgdi32 -lwinmm

int main() {
    
    Board board = InitialBoard();
    Renderer renderer(&board);
    InitWindow(renderer.width, renderer.height, "Chess Opening Learning Tool");
    SetExitKey(KEY_NULL);
    renderer.InitPieceTextures();
    
    SetTargetFPS(30);
    Input input = InitialInput();
    while (!WindowShouldClose()) {
        
        HandleInput(board, input, BOARD_SIZE_PIXELS);
        if (input.endProgram) {
            break;
        }
        renderer.SetHighlighted(input.state == PIECE_SELECTED, input.selectedX, input.selectedY);
        renderer.SetPieceToPlace(input.toPlace);
        
        BeginDrawing();
            
            renderer.Draw();

        EndDrawing();
    }
    
    CloseWindow();
    
    return 0;
}