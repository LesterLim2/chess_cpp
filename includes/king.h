#pragma once

#include <iostream>
#include "piece.h"
#include "dangerMap.h"
class Board;
class DangerMap;
class King : public Piece{
private:
    std::vector<int> castleCols = {0,7};
public:
    King(ColorType color, std::pair<int,int> position);
    std::string checkMovement(Board& board) override;
    std::string checkMovement(Board& board, DangerMap& dangerMap);
    std::string checkCastle(Board& board, DangerMap& dangerMap, std::string& availableCastles);
    std::vector<std::pair<int,int>> getDanger(Board& board) override;
};