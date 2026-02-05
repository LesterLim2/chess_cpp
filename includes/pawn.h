#pragma once

#include <piece.h>
#include <utility>
#include <vector>

class Board;

class Pawn : public Piece{
private:
    bool hasMoved = false;
    std::vector<std::pair<std::pair<int,int>,moveType>> availableSquaresToMove = {};
    //change this later
public:
    Pawn(ColorType color, std::pair<int,int> positon);
    void stateType() override;
    void checkMovement(Board& board);
    
    void checkCapture(Board& board);

    void promotion();
};