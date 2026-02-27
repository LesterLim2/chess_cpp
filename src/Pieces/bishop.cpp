#include <iostream>
#include "utility"
#include "bishop.h"
#include "board.h"
#include "dangerMap.h"


Bishop::Bishop(ColorType color, std::pair<int,int> position)
    : Piece(color, PieceType::Bishop, position) {
    directions = {{1,1},{1,-1},{-1,1},{-1,-1}};
    isSliding = true;
}



