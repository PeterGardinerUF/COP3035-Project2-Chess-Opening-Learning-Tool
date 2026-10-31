#include "renderer.hpp"
#include "raylib.h"
#include <iostream>

void Renderer::SetHighlighted(bool exists, int x, int y) {
    highlightedExists = exists;
    highlightedX = x;
    highlightedY = y;
}

void Renderer::SetPieceToPlace(Piece piece) {
    toPlace = piece;
}

Renderer::Renderer(Board* board) {
    this->board = board;
    highlightedExists = false;
    toPlace = EMPTY;
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
    }
}

void Renderer::InitTexture(Texture2D& texture, const char* filePath) const {
    Image image = LoadImage(filePath);
    texture = LoadTextureFromImage(image);
    UnloadImage(image);
}

void Renderer::DrawBoard(Board* board, int topLeftCornerX, int topLeftCornerY, int squareSize, bool showHighlight, int highlightX, int highlightY) const {
    for (int i = 0; i < 64; i++) {
        Color color = (i + i / 8) % 2 == 0 ? BEIGE : BROWN;
        DrawRectangle((i % 8) * squareSize + topLeftCornerX, (i / 8) * squareSize + topLeftCornerY, squareSize, squareSize, color);
    }
    
    if (showHighlight) {
        Color color = PURPLE;
        color.a = 128;
        DrawRectangle(highlightX * squareSize + topLeftCornerX, (7 - highlightY) * squareSize + topLeftCornerY, squareSize, squareSize, color);
    }
    
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            Piece piece = board->pieces[x][y];
            if (piece >= EMPTY) continue;
            Texture texture = pieceTextures[piece];
            texture.width = squareSize;
            texture.height = squareSize;
            DrawTexture(texture, x * squareSize + topLeftCornerX, (7 - y) * squareSize + topLeftCornerY, WHITE);
        }
    }
}

void Renderer::DrawContinuation(int x, const ContinuationData& continuation) const {
    if (!continuation.exists) {
        DrawText("NOT FOUND", x, 300 + BOARD_SIZE_PIXELS / 4, 20, RED);
        return;
    }
    
    Board newBoard = *board;
    
    ApplyContinuation(newBoard, continuation.continuation);
    
    DrawBoard(&newBoard, x, 300, BOARD_SIZE_PIXELS / 16 / 2, true, continuation.continuation.toX, continuation.continuation.toY);
    uint32_t totalGames = continuation.whiteWins + continuation.blackWins + continuation.draws;
    double whiteFraction = totalGames == 0 ? 0 : (double)continuation.whiteWins / totalGames;
    double blackFraction = totalGames == 0 ? 0 : (double)continuation.blackWins / totalGames;
    double drawFraction  = totalGames == 0 ? 0 : (double)continuation.draws / totalGames;
    string whiteText = "White Wins: " + to_string(whiteFraction * 100).substr(0, 4) + '%';
    string blackText = "Black Wins: " + to_string(blackFraction * 100).substr(0, 4) + '%';
    string drawText  = "Draw: " + to_string(drawFraction * 100).substr(0, 4) + '%';
    DrawText(whiteText.data(), x, 300 + BOARD_SIZE_PIXELS / 4, 20, GREEN);
    DrawText(blackText.data(), x, 320 + BOARD_SIZE_PIXELS / 4, 20, GREEN);
    DrawText(drawText.data(), x, 340 + BOARD_SIZE_PIXELS / 4, 20, GREEN);
    string gameCountText = "Game count:\n" + to_string(totalGames);
    DrawText(gameCountText.data(), x, 360 + BOARD_SIZE_PIXELS / 4, 20, GREEN);
}

void Renderer::Draw(const bool usingTrie) {
    ClearBackground(BLACK);
    DrawBoard(board, 0, 0, squareSize, highlightedExists, highlightedX, highlightedY);
    
    int buttonSize = BOARD_SIZE_PIXELS / 16;
    DrawRectangle(BOARD_SIZE_PIXELS, 0, buttonSize * UNIQUE_PIECE_COUNT, buttonSize, GRAY);
    if (toPlace != EMPTY) DrawRectangle(BOARD_SIZE_PIXELS + buttonSize * (int)toPlace, 0, buttonSize, buttonSize, GREEN);
    for (int i = 0; i < UNIQUE_PIECE_COUNT; i++) {
        Texture2D texture = pieceTextures[i];
        texture.width = squareSize / 2;
        texture.height = squareSize / 2;
        DrawTexture(texture, BOARD_SIZE_PIXELS + i * buttonSize, 0, WHITE);
    }
    
    int playerButtonX = BOARD_SIZE_PIXELS;
    int playerButtonY = buttonSize;
    DrawRectangle(playerButtonX, playerButtonY, 300, fontSize, GRAY);
    if (board->whiteToPlay) {
        DrawText("White to play", playerButtonX, playerButtonY, fontSize, WHITE);
    } else {
        DrawText("Black to play", playerButtonX, playerButtonY, fontSize, BLACK);
    }
    
    int dataStructureToggleX = BOARD_SIZE_PIXELS;
    int dataStructureToggleY = BOARD_SIZE_PIXELS / 16 * 4;
    DrawRectangle(dataStructureToggleX, dataStructureToggleY, 250, fontSize, GRAY);
    if (usingTrie) {
        DrawText("Trie", dataStructureToggleX, dataStructureToggleY, fontSize, BLACK);
    } else {
        DrawText("Hash Map", dataStructureToggleX, dataStructureToggleY, fontSize, BLACK);
    }
    
    for (int i = 0; i < 3; i++) {
        DrawContinuation(BOARD_SIZE_PIXELS + 20 + (BOARD_SIZE_PIXELS / 4 + 20) * i, continuations[i]);
    }
    
}
