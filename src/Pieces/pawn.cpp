#include "pawn.h"
#include <iostream>

using namespace std;

Pawn::Pawn(ColorType c, pair<int,int> p)
    : Piece(c, PieceType::Pawn, p) {}

void Pawn::stateType() {
    std::cout << "I am a pawn\n";
}