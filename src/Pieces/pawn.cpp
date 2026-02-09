#include "pawn.h"
#include "board.h"
#include "types.h"

#include <iostream>
#include <utility>
#include <vector>

Pawn::Pawn(ColorType c, std::pair<int,int> p)
    : Piece(c, PieceType::Pawn, p, {{1,1},{-1,1}}) {}


void Pawn::checkMovement(Board& board){
    int x  = position.first;
    int y = position.second;
    ColorType color = this->color;

    int dir = (color == ColorType::White) ? 1 : -1;

    if (board.checkSquareAvailability(x, y + dir)) {
        availableSquaresToMove.push_back({{x, y + dir},moveType::Move}); //moveType is kinda irrelevant ngl
    }

    if (!hasMoved && board.checkSquareAvailability(x, y + dir) && board.checkSquareAvailability(x, y + (dir * 2))) {
        availableSquaresToMove.push_back({{x, y + (dir * 2)},moveType::Move});
    }

    if (availableSquaresToMove.empty()){
        std::cout << "no available squares for pawn to move/capture" << std::endl;
    }
} 


void Pawn::checkCapture(Board& board){
    int x = position.first;
    int y = position.second;
    ColorType color = this->color;

    int dir = (color == ColorType::White) ? 1 : -1;
    int curSize = static_cast<int>(availableSquaresToMove.size());

    if (board.isAvailableToCapture(color,x + 1, y + dir)){
        availableSquaresToMove.push_back({{x + 1,y + dir},moveType::Capture});
        std::cout << "piece is available for capture at coordinates " << x + 1 << ", " << y + dir<< std::endl;
    }
    if (board.isAvailableToCapture(color,x - 1, y + dir)){
        availableSquaresToMove.push_back({{x - 1,y + dir},moveType::Capture});
        std::cout << "piece is available for capture at coordinates " << x - 1 << ", " << y + dir << std::endl;                                  
    }
    if (curSize == availableSquaresToMove.size()){
        std::cout << "no available pieces to capture";
    }
}


void Pawn::addDangerTiles(Board& board){
    int curX = position.first;
    int curY = position.second;
    dangerTiles.push_back({curX,curY});

    for (auto& dir : danger){
        int x = dir.first;
        int y = dir.second;
        if (board.isInBounds(curX + x,curY + y)){
            dangerTiles.push_back({curX + x, curY + y});
        }
    }
}


void Pawn::stateType(){
    std::cout << "I am a pawn ";
}
