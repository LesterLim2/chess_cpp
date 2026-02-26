#include "pawn.h"
#include "board.h"

#include <iostream>
#include <utility>
#include <vector>

Pawn::Pawn(ColorType c, std::pair<int,int> p)
    : Piece(c, PieceType::Pawn, p) {
    determineDirection();
    determineDanger();
}

void Pawn::determineDirection(){
    directions = color == ColorType::White ?
        std::vector<std::pair<int,int>>{{0,1}} :
        std::vector<std::pair<int,int>>{{0,-1}};
}

void Pawn::determineDanger(){
    this->danger = color == ColorType::White ?
        std::vector<std::pair<int,int>>{{1,1},{-1,1}} :
        std::vector<std::pair<int,int>>{{1,-1},{-1,-1}};
}

std::string Pawn::checkMovement(Board& board){
    int x = position.second;
    int y = position.first;
    int dir = (color == ColorType::White) ? 1 : -1;
    std::string availableMoves = "";
    std::string availableCaptures = "";

    if (board.checkSquareAvailability(y + dir, x)) {
        availableMoves += (y + dir) + '0';
        availableMoves += x + '0';
        availableMoves += '&';
        if (!hasMoved && board.checkSquareAvailability(y + dir * 2, x)) {
            availableMoves += (y + dir * 2) + '0';
            availableMoves += x + '0';
            availableMoves += '&';
        }
    }

    for (auto& d : danger){
        int captureY = y + d.second;
        int captureX = x + d.first;
        if (board.isAvailableToCapture(color, captureY, captureX)){
            availableCaptures += captureY + '0';
            availableCaptures += captureX + '0';
            availableCaptures += '&';
        }
    }

    return availableMoves + "c" + availableCaptures;
}

//called when pawn moves to end of the board
void Pawn::promotion(Board& board){

}
void Pawn::stateType(){
    std::cout << "I am a pawn ";
}
