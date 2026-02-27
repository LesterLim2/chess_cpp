#pragma once

#include <iostream>
#include "piece.h"
class Board;
class King : public Piece{
public:
    King(ColorType color, std::pair<int,int> position);
    std::string checkMovement(Board& board);
};