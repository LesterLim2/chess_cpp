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
    std::string availableEnPassant = "";

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

    if(this->color == ColorType::White ? y == 4 : y == 3){
        for(int xDir: adjacacentHorizontalTiles){
            Piece* piece = board.getPiece(x + xDir,y);
            if (piece == nullptr) continue;
            if((this->color != piece->getColor() && piece->getFirstMovement() == 2)){
                availableEnPassant += (y + dir) + '0';
                availableEnPassant += (x + xDir) + '0';
                availableEnPassant += '&';
            }
        }
    }

    //check en passant
    
    return availableMoves + "c" + availableCaptures + (availableEnPassant.empty() ? "" : "n" + availableEnPassant);
}

std::string Pawn::checkPreMoves(Board& board){
    std::string movement = checkMovement(board);
    return movement.find('&') != std::string::npos ? movement : "None";
}

//called when pawn moves to end of the board
void Pawn::promotion(Board& board){

}
void Pawn::stateType(){
    std::cout << "I am a pawn ";
}
