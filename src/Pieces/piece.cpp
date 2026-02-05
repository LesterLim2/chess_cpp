#include <iostream>
#include <vector>
#include "types.h"
#include "piece.h"
#include "board.h"

using namespace std;

//constructor for regular pieces, position is loaded at 10 initially to simulate out of bounds
Piece::Piece(ColorType color, PieceType type, std::pair<int,int> position)
    : color(color), type(type), position(position) {}

Piece::Piece(std::pair<int,int> position)
    : color(ColorType::None), type(PieceType::None), position(position) {}

void Piece::stateType() {
    std::cout << "I am a generic piece\n";
}
ColorType Piece::getColor(){
    return color;
}

void Piece::statePosition() {
    int x = position.first;
    int y = position.second;
    std::cout << "I am at (" << x << ", " << y << ")" << std::endl;
}
pair<int,int> Piece::getPosition(){
    return this->position;
}

//when moving, first set the position of the piece object to the new coordinates (int x, int y), then in a seperate function call (probably in the player class) call
void Piece::setPositon(int x,int y){
    position = {x, y};
}

void Piece::move(){
    cout << "I am moving!";
}
