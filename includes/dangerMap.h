#pragma once

#include <iostream>
#include <vector>

#include "piece.h"
#include "types.h"

class Board;

class Piece;

class DangerMap{
private:
    using dangerType = std::vector<ColorType>;
    std::vector<std::vector<dangerType>> dangerMap;
public:

    DangerMap();
    void insertDanger(Piece* piece, Board& board);

    void removeDanger(int x, int y, ColorType color);
    
    void updateDanger(Piece* piece,int newX,int newY);

    void stateDangerMap();

    bool hasDangerColor(int x, int y, ColorType c) const;
    int getThreatCount(int x, int y, ColorType c) const;
    int getTotalThreats(int x, int y) const;
};
