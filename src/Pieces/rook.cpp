#include <iostream>

#include "rook.h"
#include "board.h"

Rook::Rook(ColorType c, std::pair<int,int> p) : Piece(c,PieceType::Rook,p){
    directions = {{1,0},{-1,0},{0,1},{0,-1}};
    isSliding = true;
}

std::string Rook::checkPreMoves(Board& board){
    return checkMovement(board);
}


