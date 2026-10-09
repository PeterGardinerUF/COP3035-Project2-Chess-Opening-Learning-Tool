#pragma once
#include <iostream>
#include "game.hpp"
#include "raylib.h"
using namespace std;

class Renderer {
    
    Game* game;
    
    Texture2D pieceTextures[UNIQUE_PIECE_COUNT];
    
    void InitTexture(Texture2D& texture, const char* filePath) const;
    
    public:
    
    const int width = 1408;
    const int height = 704;
    const int squareSize = height / 8;
    
    Renderer(Game* game);
    ~Renderer();
    
    void InitPieceTextures();
    
    void Draw() const;
    
};