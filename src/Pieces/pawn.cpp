#include "pawn.h"
#include "board.h"

#include <iostream>
#include <utility>
#include <vector>

Pawn::Pawn(ColorType c, std::pair<int,int> p)
    : Piece(c, PieceType::Pawn, p, 
            c == ColorType::White ? 
                std::vector<std::pair<int,int>>{{1,1},{-1,1}} :   
                std::vector<std::pair<int,int>>{{1,-1},{-1,-1}}   
           ) {}


std::vector<std::pair<int,int>> Pawn::checkPreMoves(Board& board){
    std::vector<std::pair<int,int>> availablePreMoves = {};
    checkMovement(board,availablePreMoves);
    checkCapture(board,availablePreMoves);
    return availablePreMoves;
}

void Pawn::checkMovement(Board& board, std::vector<std::pair<int,int>>& availablePreMoves){
    int x = position.second; // col
    int y = position.first;  // row
    ColorType color = this->color;

    int dir = (color == ColorType::White) ? 1 : -1;

    if (board.checkSquareAvailability(y + dir, x)) {
        availablePreMoves.push_back({y + dir, x});
    }

    if (!hasMoved && board.checkSquareAvailability(y + dir, x) && board.checkSquareAvailability(y + (dir * 2), x)) {
        availablePreMoves.push_back({y + dir * 2, x});
    }

    if (availablePreMoves.empty()){
        std::cout << "no available squares for pawn to move/capture" << std::endl;
    }
}

void Pawn::checkCapture(Board& board, std::vector<std::pair<int,int>>& availablePreMoves){
    int x = position.second; // col
    int y = position.first;  // row
    ColorType color = this->color;
    std::cout << x << y << std::endl;

    int dir = (color == ColorType::White) ? 1 : -1;
    int curSize = static_cast<int>(availablePreMoves.size());

    if (board.isAvailableToCapture(color, y + dir, x + 1)){
        availablePreMoves.push_back({y + dir, x + 1});
        std::cout << "piece is available for capture at coordinates " << y + dir << ", " << x + 1 << std::endl;
    }
    if (board.isAvailableToCapture(color, y + dir, x - 1)){
        availablePreMoves.push_back({y + dir, x - 1});
        std::cout << "piece is available for capture at coordinates " << y + dir << ", " << x - 1 << std::endl;
    }
    if (curSize == static_cast<int>(availablePreMoves.size())){
        std::cout << "no available pieces to capture";
    }
}


void Pawn::addDangerTiles(Board& board){
    int curX = position.second;
    int curY = position.first;

    for (auto& dir : danger){
        int x = dir.first;
        int y = dir.second;
        if (board.isInBounds(curX + x,curY + y)){
            Piece* piece = board.getPiece(curX + x,curY + y);
            if (piece == nullptr){
                dangerTiles.push_back({curX + x, curY + y});
            }
            else{
                if (piece->getColor() != color){
                    if (piece->getType() == PieceType::King){
                        //checkmate logic
                    }
                    dangerTiles.push_back({curX + x,curY + y});
                }
            }
        }
    }
}

void Pawn::stateType(){
    std::cout << "I am a pawn ";
}
