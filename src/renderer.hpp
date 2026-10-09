#pragma once
#include <iostream>
#include "game.hpp"
using namespace std;

class Renderer {
    
    Game* game;
    
    public:
    
    Renderer(Game* game);
    
    void CommandLineDraw();
    
    void Draw();
    
};