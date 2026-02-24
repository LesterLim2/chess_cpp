#include <iostream>

#include "queen.h"

Queen::Queen(ColorType color, std::pair<int,int> position)
    : Piece(color,PieceType::Queen,position){
        directions = {{1,0},{-1,0},{0,1},{0,-1},{1,1},{-1,1},{-1,1},{-1,-1}};
    };