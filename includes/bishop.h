#pragma once

#include <iostream>

#include "piece.h"

class Board;

class Bishop : Piece{
private:
    void addDangerTiles(Board& board) override;
public:


};