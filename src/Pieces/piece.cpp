#include <iostream>
#include <vector>

#include "piece.h"
#include "board.h"
#include "dangerMap.h"



Piece::Piece(ColorType color, PieceType type, std::pair<int,int> position)
    : color(color), type(type), position(position) {}


Piece::Piece(std::pair<int,int> position)
    : color(ColorType::None), type(PieceType::None), position(position) {}


void Piece::stateType() {
    std::cout << "I am a generic piece\n";
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

std::string Piece::checkPreMoves(Board& board){
    std::string availablePreMoves = checkMovement(board);
    return availablePreMoves != "c" ? availablePreMoves : "None";
}

std::string Piece::checkMovement(Board& board){
    int y = position.first;
    int x = position.second;
    std::string availableMoves = "";
    std::string availableCaptures = "";

    for(auto& dir : directions){
        int dy = dir.second;
        int dx = dir.first;
        int curY = y + dy;
        int curX = x + dx;
        while(board.isInBounds(curY, curX)){
            if(board.checkSquareAvailability(curY, curX)){
                availableMoves += curY + '0';
                availableMoves += curX + '0';
                availableMoves += '&';
                curY += dy;
                curX += dx;
            }
            else{
                if(board.isAvailableToCapture(color, curY, curX)){
                    availableCaptures += curY + '0';
                    availableCaptures += curX + '0';
                    availableCaptures += '&';
                }
                break;
            }
        }
    }
    return availableMoves + "c" + availableCaptures;
}

std::string Piece::checkThreats(Board& board){
    int y = position.first;
    int x = position.second;
    std::string availableThreats = "";
    for (auto& d : danger){
        int threatY = y + d.second;
        int threatX = x + d.first;
        if (board.isInBounds(threatY, threatX)){
            Piece* piece = board.getPiece(threatY, threatX);
            if (piece == nullptr || piece->getColor() != color){
                availableThreats += threatY + '0';
                availableThreats += threatX + '0';
                availableThreats += '&';
            }
        }
    }
    std::cout << availableThreats << std::endl;
    return availableThreats;
}

void Piece::setHasMoved(bool hasMoved){
    this->hasMoved = hasMoved;
}

bool Piece::getHasMoved(){
    return hasMoved;
}

bool Piece::getIsSliding(){
    return isSliding;
}

std::vector<std::pair<int,int>> Piece::getDirections(){
    return directions;
}