#include "board.hpp"
#include "renderer.hpp"
#include "input.hpp"
#include "hash_map.hpp"
#include "chess_tries.h" // RT
#include "move_popup.hpp"
// g++ src/*.cpp -o build/debug.exe -Wall -Wextra -g -IC:/raylib/raylib/src -LC:/raylib/raylib/src -lraylib -lopengl32 -lgdi32 -lwinmm

const string GAMES_FILE = "games.txt";
const int MAX_GAMES = 100000; // Number of games to load
const int MAX_PLY = 8; // Number of half-moves deep to store (2 ply: e2e4 e7e5)

int main() {

    // Initializes the trie and loads all games into it
    Trie trie;
    int loaded = loadFile(trie, GAMES_FILE, MAX_GAMES, MAX_PLY);
    if (loaded == 0) {
        cout << "No games loaded. Check that " << GAMES_FILE << " exists" << endl;
        return 1;
    }
    cout << "Loaded " << loaded << " games into trie successfully." << endl;
    
    Board board = InitialBoard();
    Renderer renderer(&board);
    InitWindow(renderer.width, renderer.height, "Chess Opening Learning Tool");
    SetExitKey(KEY_NULL);
    renderer.InitPieceTextures();
    
    SetTargetFPS(30);
    Input input = InitialInput();
    // RT
    HashMap hashMap;
    vector<string> moves;

    while (!WindowShouldClose()) {
        
        HandleInput(board, input, BOARD_SIZE_PIXELS);
        if (input.endProgram) {
            break;
        } else if (input.boardModified) {
            // RT
            if (input.usingTrie == true) {
                trie.find(moves);
            } else {
                BoardData data;
                hashMap.Find(board, &data);
            }
        }
        renderer.SetHighlighted(input.state == PIECE_SELECTED, input.selectedX, input.selectedY);
        renderer.SetPieceToPlace(input.toPlace);
        
        BeginDrawing();
            
            renderer.Draw(input.usingTrie);
            DrawLastMove();

        EndDrawing();
    }
    
    CloseWindow();
    
    return 0;
}
