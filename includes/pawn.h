#pragma once

#include <piece.h>
#include <utility>


class Pawn : public Piece{
public:
    Pawn(ColorType color, std::pair<int,int> positon);
    void stateType() override;
};