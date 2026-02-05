#pragma once
#include "types.h"
#include "board.h"
#include <utility>

class Piece {
protected:
    ColorType color;
    PieceType type;
    std::pair<int,int> position;

public:
    Piece(ColorType color = ColorType::None,
          PieceType type = PieceType::None,
          std::pair<int,int> position = {10,10});

    Piece(std::pair<int,int> position);   // no default needed
    virtual ~Piece() = default;

    virtual void stateType();

    std::pair<int,int> getPosition();
    void statePosition();
    void setPositon(int x,int y);

    ColorType Piece::getColor();

    virtual void move();
};
