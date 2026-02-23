#pragma once

#include <iostream>

#include "piece.h"

class Board;

class Bishop :public Piece{
private:
    std::vector<std::pair<int,int>> directions = {{1,1},{1,-1},{-1,1},{-1,-1}};
public:
    Bishop(ColorType color,std::pair<int,int> position);
    void addDangerTiles(Board& board) override;

    // std::vector<std::pair<int,int>> checkPreMoves(Board& board) override;

    // void checkCapture(Board& board,std::vector<std::pair<int,int>>& availablePreMoves);
    // void checkMovement(Board& board,std::vector<std::pair<int,int>>& availablePreMoves);
};