#include <iostream>
#include <vector>

#include "piece.h"


Piece::Piece(ColorType color, PieceType type, std::pair<int,int> position,std::vector<std::pair<int,int>> danger)
    : color(color), type(type), position(position), danger{danger} {}


Piece::Piece(std::pair<int,int> position)
    : color(ColorType::None), type(PieceType::None), position(position) {}


void Piece::stateType() {
    std::cout << "I am a generic piece\n";
}


void Piece::addDangerTiles(Board& board){
    std::cout << "i'm not supposed to do anything yet!" << std::endl;
}


std::vector<std::pair<int,int>> Piece::getDangerTiles(){
    return dangerTiles;
}

void Piece::setDangerTiles(std::vector<std::pair<int,int>> tiles){
    dangerTiles = tiles;
}



std::vector<std::pair<int,int>> Piece::getDanger(){
    return danger;
}


PieceType Piece::getType(){
    return type;
}


ColorType Piece::getColor(){
    return color;
}


void Piece::statePosition() {
    int x = position.first;
    int y = position.second;
    std::cout << "I am at (" << x << ", " << y << ")" << std::endl;
}

std::pair<int,int> Piece::getPosition(){
    return this->position;
}

void Piece::setPosition(int x, int y){
    position = {x, y};
}
