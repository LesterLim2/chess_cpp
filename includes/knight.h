#pragma once
#include <iostream>

#include "piece.h"

class Board;

class Knight : public Piece{
public:
    Knight(ColorType color, std::pair<int, int> position);
    std::string checkMovement(Board& board) override;
};