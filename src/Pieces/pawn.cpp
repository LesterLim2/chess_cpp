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

    if (board.checkSquareAvailability(x, y + 1)) {
        availableSquaresToMove.push_back({{x, y + 1},moveType::Move});
    }

    if (!hasMoved && board.checkSquareAvailability(x, y + 2)) {
        availableSquaresToMove.push_back({{x, y + 2},moveType::Move});
    }

    if (availableSquaresToMove.empty()){
        cout << "no available squares for pawn to move/capture" << endl;
    }
} 

void Pawn::checkCapture(Board& board){
    int x = position.first;
    int y = position.second;
    
    if (board.isAvailableToCapture(color,x + 1, y + 1)){
        availableSquaresToMove.push_back({{x,y + 1},moveType::Capture});
        cout << "piece is available for capture at coordinates" << x  + 1<< y + 1<< endl;
    }
    if (board.isAvailableToCapture(color,x - 1, y + 1)){
        availableSquaresToMove.push_back({{x,y + 1},moveType::Capture});
        cout << "piece is available for capture at coordinates" << x - 1 << y + 1 << endl;
    }
}


void Pawn::stateType(){
    std::cout << "I am a pawn ";
}

