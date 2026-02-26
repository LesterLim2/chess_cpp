#pragma once

#include "piece.h"
#include <utility>
#include <vector>

class Board;
class DangerMap;

class Pawn : public Piece{
private:
    std::vector<std::pair<std::pair<int,int>,moveType>> availableSquaresToMove = {};

    void determineDirection();
    void determineDanger();

public:
    Pawn(ColorType color, std::pair<int,int> positon);
    void stateType() override;

    void promotion(Board& board);
    std::string checkMovement(Board& board) override;


};