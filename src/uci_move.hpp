#pragma once
#include <string>
using namespace std;

// This file exists to keep track of which moves are actually being made (in UCI notation) by the player over the board. 
// This is necessary for the trie implementation, which needs to know exactly what moves the player is making so it can follow along by stepping down the appropriate branches.

// Converts the x and y coordinates of a square into its proper notation (ex: 00 -> a1)
inline string SquareName(int x, int y) {
    string s;
    s += static_cast<char>('a' + x);
    s += static_cast<char>('1' + y);
}

// Returns a string representing a move in UCI notation, given its start and end coordinates
inline string MakeUCIMove (int fromX, int fromY, int toX, int toY, char promotion = 0) {
    string move = SquareName(fromX, fromY) + SquareName(toX, toY);
    if (promotion != 0) {
        move += promotion;
    }
    return move;
}