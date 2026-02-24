#pragma once

#include "piece.h"
#include <utility>
#include <vector>

class Board;
class DangerMap;

class Pawn : public Piece{
private:
    bool hasMoved = false;
    std::vector<std::pair<int,int>> direction;
    std::vector<std::pair<std::pair<int,int>,moveType>> availableSquaresToMove = {};

    void determineDirection();
    void determineDanger();

public:
    Pawn(ColorType color, std::pair<int,int> positon);
    void stateType() override;
    std::vector<std::pair<int,int>> checkCapture(Board& board) override;

    void promotion(Board& board);

    void addDangerTiles(Board& board,DangerMap& dangerMap) override;
    std::vector<std::pair<int,int>> checkMovement(Board& board) override;

    
};