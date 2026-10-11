#include "renderer.hpp"

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
    ClearBackground(BLACK);
    for (int i = 0; i < 64; i++) {
        Color color = (i + i / 8) % 2 == 0 ? BEIGE : BROWN;
        DrawRectangle((i % 8) * squareSize, (i / 8) * squareSize, squareSize, squareSize, color);
    }
    
    if (highlightedExists) {
        Color color = PURPLE;
        color.a = 128;
        DrawRectangle(highlightedX * squareSize, (7 - highlightedY) * squareSize, squareSize, squareSize, color);
    }
    
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            Piece piece = board->pieces[x][y];
            if (piece == EMPTY) continue;
            DrawTexture(pieceTextures[piece], x * squareSize, (7 - y) * squareSize, WHITE);
        }
    }
    
    int buttonSize = BOARD_SIZE_PIXELS / 16;
    DrawRectangle(BOARD_SIZE_PIXELS, 0, buttonSize * UNIQUE_PIECE_COUNT, buttonSize, GRAY);
    if (toPlace != EMPTY) DrawRectangle(BOARD_SIZE_PIXELS + buttonSize * (int)toPlace, 0, buttonSize, buttonSize, GREEN);
    for (int i = 0; i < UNIQUE_PIECE_COUNT; i++) {
        Texture2D texture = pieceTextures[i];
        texture.width /= 2;
        texture.height /= 2;
        DrawTexture(texture, BOARD_SIZE_PIXELS + i * buttonSize, 0, WHITE);
    }
}
