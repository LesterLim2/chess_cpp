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


};