#include <iostream>
#include "game.hpp"
#include "renderer.hpp"
using namespace std;
// g++ src/*.cpp -o build/debug.exe -Wall -Wextra

int main() {
    Game game = InitialGame();
    Renderer renderer(&game);
    renderer.CommandLineDraw();
    return 0;
}