#include "pawn.h"
#include "board.h"
#include "types.h"

#include <iostream>
#include <utility>
#include <vector>

using namespace std;

Pawn::Pawn(ColorType c, pair<int,int> p)
    : Piece(c, PieceType::Pawn, p) {}

void Pawn::checkMovement(Board& board){
    int x  = position.first;
    int y = position.second;
    ColorType color = this->color;

    int dir = (color == ColorType::White) ? 1 : -1;

    if (board.checkSquareAvailability(x, y + dir)) {
        availableSquaresToMove.push_back({{x, y + dir},moveType::Move});
    }

    if (!hasMoved && board.checkSquareAvailability(x, y + dir) && board.checkSquareAvailability(x, y + (dir * 2))) {
        availableSquaresToMove.push_back({{x, y + (dir * 2)},moveType::Move});
    }

    if (availableSquaresToMove.empty()){
        cout << "no available squares for pawn to move/capture" << endl;
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
        cout << "piece is available for capture at coordinates " << x + 1<< y + dir<< endl;
    }
    if (board.isAvailableToCapture(color,x - 1, y + dir)){
        availableSquaresToMove.push_back({{x - 1,y + dir},moveType::Capture});
        cout << "piece is available for capture at coordinates " << x - 1 << y + dir << endl;                                  
    }
    if (curSize == availableSquaresToMove.size()){
        cout << "no available pieces to capture";
    }
}


void Pawn::stateType(){
    std::cout << "I am a pawn ";
}

