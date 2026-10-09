#include "renderer.hpp"
#include "game.hpp"
#include "raylib.h"

Renderer::Renderer(Game* game) {
    this->game = game;
}

Renderer::~Renderer() {
    for (int i = 0; i < UNIQUE_PIECE_COUNT; i++) {
        UnloadTexture(pieceTextures[i]);
    }
}

void Renderer::InitPieceTextures() {
    // These are in the same order of the enum, so pieceTextures[WHITE_PAWN] will give white pawn data.
    const char* imageFilePaths[12] = {
        "assets/images/white-pawn.png",
        "assets/images/white-knight.png",
        "assets/images/white-bishop.png",
        "assets/images/white-rook.png",
        "assets/images/white-queen.png",
        "assets/images/white-king.png",
        "assets/images/black-pawn.png",
        "assets/images/black-knight.png",
        "assets/images/black-bishop.png",
        "assets/images/black-rook.png",
        "assets/images/black-queen.png",
        "assets/images/black-king.png",
    };
    
    for (int i = 0; i < UNIQUE_PIECE_COUNT; i++) {
        InitTexture(pieceTextures[i], imageFilePaths[i]);
        pieceTextures[i].width = squareSize;
        pieceTextures[i].height = squareSize;
    }
}

void Renderer::InitTexture(Texture2D& texture, const char* filePath) const {
    Image image = LoadImage(filePath);
    texture = LoadTextureFromImage(image);
    UnloadImage(image);
}

void Renderer::Draw() const {
    for (int i = 0; i < 64; i++) {
        Color color = (i + i / 8) % 2 == 0 ? BEIGE : BROWN;
        DrawRectangle((i % 8) * squareSize, (i / 8) * squareSize, squareSize, squareSize, color);
    }
    
    for (int i = 0; i < 32; i++) {
        Piece piece = game->pieces[i];
        DrawTexture(pieceTextures[piece.type], (piece.x - 1) * squareSize, (piece.y - 1) * squareSize, WHITE);
    }
}
