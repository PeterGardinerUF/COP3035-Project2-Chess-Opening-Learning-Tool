#include <iostream>
#include "game.hpp"
#include "renderer.hpp"
using namespace std;
// g++ src/*.cpp -o build/debug.exe -Wall -Wextra -IC:/raylib/raylib/src -LC:/raylib/raylib/src -lraylib -lopengl32 -lgdi32 -lwinmm

int main() {
    
    Game game = InitialGame();
    Renderer renderer(&game);
    InitWindow(renderer.width, renderer.height, "Chess Opening Learning Tool");
    renderer.InitPieceTextures();

    SetTargetFPS(30);
    
    while (!WindowShouldClose()) {
        BeginDrawing();

            ClearBackground(GRAY);
            
            renderer.Draw();

        EndDrawing();
    }
    
    CloseWindow();
    
    return 0;
}