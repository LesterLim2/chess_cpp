#pragma once

#include "piece.h"
#include <utility>
#include <vector>

class Board;

class Pawn : public Piece{
private:
    bool hasMoved = false;
    std::vector<std::pair<std::pair<int,int>,moveType>> availableSquaresToMove = {};

public:
    Pawn(ColorType color, std::pair<int,int> positon);
    void stateType() override;
    void checkCapture(Board& board,std::vector<std::pair<int,int>>& availablePreMoves);

    void promotion();

    void addDangerTiles(Board& board) override;
    void checkMovement(Board& board,std::vector<std::pair<int,int>>& availablePreMoves);

    std::vector<std::pair<int,int>> checkPreMoves(Board& board) override;
    
};