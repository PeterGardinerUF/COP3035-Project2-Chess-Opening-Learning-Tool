#pragma once
#include "raylib.h"
#include "board.hpp"

#define BOARD_SIZE_PIXELS 704

class Renderer {
    
    Board* board;
    bool highlightedExists;
    int highlightedX;
    int highlightedY;
    Piece toPlace;
    
    Texture2D pieceTextures[UNIQUE_PIECE_COUNT];
    
    void InitTexture(Texture2D& texture, const char* filePath) const;
    
    void DrawBoard(Board* board, int topLeftCornerX, int topLeftCornerY, int squareSize, bool showHighlight, int highlightX, int highlightY) const;
    
    const int fontSize = 44;
    
    public:
    
    struct ContinuationData {
        bool exists;
        uint32_t whiteWins = 0;
        uint32_t blackWins = 0;
        uint32_t draws = 0;
        Continuation continuation;
    };
    ContinuationData continuations[3];
    
    void DrawContinuation(int x, const ContinuationData& continuation) const;
    
    const int width = BOARD_SIZE_PIXELS * 2;
    const int height = BOARD_SIZE_PIXELS;
    const int squareSize = height / 8;
    
    void SetHighlighted(bool exists, int x, int y);
    void SetPieceToPlace(Piece piece);
    
    Renderer(Board* board);
    ~Renderer();
    
    void InitPieceTextures();
    
    void Draw(const bool usingTrie);
    
};