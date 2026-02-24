#pragma once

#include "piece.h"

class Queen : public Piece{
public:
    Queen(ColorType color,std::pair<int,int> position);
};