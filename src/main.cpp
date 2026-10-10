#include "game.hpp"
#include "raylib.h"
#include "renderer.hpp"
#include "input.hpp"
using namespace std;
// g++ src/*.cpp -o build/debug.exe -Wall -Wextra -IC:/raylib/raylib/src -LC:/raylib/raylib/src -lraylib -lopengl32 -lgdi32 -lwinmm

int main() {
    
    Board game = InitialGame();
    Renderer renderer(&game);
    InitWindow(renderer.width, renderer.height, "Chess Opening Learning Tool");
    SetExitKey(KEY_NULL);
    renderer.InitPieceTextures();

    SetTargetFPS(30);
    Input input = InitialInput();
    while (!WindowShouldClose()) {
        
        HandleInput(game, input, BOARD_SIZE_PIXELS);
        if (input.endProgram) {
            break;
        }
        renderer.SetHighlighted(input.state == PIECE_SELECTED, input.selectedX, input.selectedY);
        
        BeginDrawing();
            
            ClearBackground(GRAY);
            
            renderer.Draw();

        EndDrawing();
    }
    
    CloseWindow();
    
    return 0;
}