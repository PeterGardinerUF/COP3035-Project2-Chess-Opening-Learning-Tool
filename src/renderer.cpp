#include "renderer.hpp"
#include "game.hpp"

Renderer::Renderer(Game* game) {
    this->game = game;
}

void Renderer::CommandLineDraw() {
    for (int i = 0; i < 64; i++) {
        bool pieceAtSquare = false;
        PieceType type;
        for (int j = 0; j < 32; j++) {
            if (game->pieces[j].positionIndex == i && game->pieces[j].alive) {
                pieceAtSquare = true;
                type = game->pieces[j].type;
                break;
            }
        }
        if (pieceAtSquare) {
            cout << pieceSymbol(type) << ' ';
        } else {
            cout << "__ ";
        }
        
        if (i % 8 == 7) {
            cout << '\n';
        }
    }
}