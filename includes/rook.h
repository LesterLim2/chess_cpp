#pragma once
#include <iostream>

#include "piece.h"

class Board;
class DangerMap;
class Rook : public Piece{
public:
    Rook(ColorType color, std::pair<int,int> position);
    std::string checkPreMoves(Board& board) override;
};