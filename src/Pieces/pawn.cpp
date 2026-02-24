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
    this->direction = color == ColorType::White ?
        std::vector<std::pair<int,int>>{{0,1}} :
        std::vector<std::pair<int,int>>{{0,-1}};
}

void Pawn::determineDanger(){
    this->danger = color == ColorType::White ?
        std::vector<std::pair<int,int>>{{1,1},{-1,1}} :
        std::vector<std::pair<int,int>>{{1,-1},{-1,-1}};
}

std::vector<std::pair<int,int>> Pawn::checkMovement(Board& board){
    int x = position.second; // col
    int y = position.first;  // row
    int dir = (color == ColorType::White) ? 1 : -1;
    std::vector<std::pair<int,int>> availableMoves = {};

    if (board.checkSquareAvailability(y + dir, x)) {
        availableMoves.push_back({y + dir, x});
    }
    if (!hasMoved && board.checkSquareAvailability(y + dir, x) && board.checkSquareAvailability(y + (dir * 2), x)) {
        availableMoves.push_back({y + dir * 2, x});
    }
    if (availableMoves.empty()){
        std::cout << "no available squares for pawn to move" << std::endl;
    }
    return availableMoves;
}

std::vector<std::pair<int,int>> Pawn::checkCapture(Board& board){
    int x = position.second; // col
    int y = position.first;  // row
    int dir = (color == ColorType::White) ? 1 : -1;
    std::vector<std::pair<int,int>> availableCaptures = {};

    if (board.isAvailableToCapture(color, y + dir, x + 1)){
        availableCaptures.push_back({y + dir, x + 1});
        std::cout << "piece available for capture at " << y + dir << ", " << x + 1 << std::endl;
    }
    if (board.isAvailableToCapture(color, y + dir, x - 1)){
        availableCaptures.push_back({y + dir, x - 1});
        std::cout << "piece available for capture at " << y + dir << ", " << x - 1 << std::endl;
    }
    if (availableCaptures.empty()){
        std::cout << "no available pieces to capture" << std::endl;
    }
    return availableCaptures;
}


void Pawn::addDangerTiles(Board& board,DangerMap& dangerMap){
    int curX = position.second;
    int curY = position.first;

    for (auto& dir : danger){
        int x = dir.first;
        int y = dir.second;
        if (!board.isInBounds(curX + x,curY + y)) continue;
        Piece* piece = board.getPiece(curX + x,curY + y);
        if (piece == nullptr){
            dangerMap.insertDanger(curX + x, curY + y, color);
        }
        else{
            if (piece->getColor() != color){
                if (piece->getType() == PieceType::King){
                    //checkmate logic toDO
                }
                dangerMap.insertDanger(curX + x, curY + y, color);
            }
        }
    }
}

//called when pawn moves to end of the board
void Pawn::promotion(Board& board){

}
void Pawn::stateType(){
    std::cout << "I am a pawn ";
}
