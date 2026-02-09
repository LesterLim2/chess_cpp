#pragma once
#include "types.h"
#include "board.h"
#include <utility>
#include <vector>

class Board; 
class Piece {
protected:
    ColorType color;
    PieceType type;
    std::pair<int,int> position;
    std::vector<std::pair<int,int>> danger;
    std::vector<std::pair<int,int>> dangerTiles = {};

public:
    Piece(ColorType color = ColorType::None,
          PieceType type = PieceType::None,
          std::pair<int,int> position = {10,10},
          std::vector<std::pair<int,int>> danger = {}
                                                        );

    Piece(std::pair<int,int> position);   // no default needed
    virtual ~Piece() = default;

    virtual void stateType();
    PieceType getType();

    std::pair<int,int> getPosition();
    void statePosition();
    void setPosition(int x,int y);

    ColorType Piece::getColor();

    virtual std::vector<std::pair<int,int>> getDanger();

    virtual void addDangerTiles(Board& board);
    std::vector<std::pair<int,int>> getDangerTiles();
    void setDangerTiles(std::vector<std::pair<int,int>> tiles);
};
