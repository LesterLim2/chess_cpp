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

//function is called when piece is moved (used for checkmate threats)
void Piece::addDangerTiles(Board& board,DangerMap& dangerMap){
    if (this->directions.empty()) throw std::runtime_error("no directions inialised for piece");
    auto& [originalX,originalY] = position;
    for (auto& dir : directions){
        int x = originalX;
        int y = originalY;
        int xDir = dir.second;
        int yDir = dir.first;

        x += xDir;
        y += yDir;
        while(board.isInBounds(x,y)){
            Piece* piece = board.getPiece(x,y);
            if (piece == nullptr){
                dangerMap.insertDanger(x,y,color);
            }
            else if (piece->getColor() != color){
                if(piece->getType() == PieceType::King){
                    //checkflag
                }
                dangerMap.insertDanger(x,y,color);
                break;
            }
            else{
                break;
            }
            x += xDir;
            y += yDir;
        }
    }
}

//function is called when a piece is moved
void Piece::removeDangerTiles(DangerMap& dangerMap){
    for(auto& danger:dangerTiles){
        dangerMap.removeDanger(position.first,position.second,color);
    }
    dangerTiles = {};
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

std::string Piece::checkPreMoves(Board& board){
    std::string availablePreMoves = vectorToString(checkMovement(board)) + "c";
    availablePreMoves += vectorToString(checkCapture(board));
    return availablePreMoves != "c" ? availablePreMoves : "None";
}

std::vector<std::pair<int,int>> Piece::checkMovement(Board& board){
    int y = position.first;
    int x = position.second;
    std::vector<std::pair<int,int>> availableMoves = {};

    for(auto& dir : directions){
        int dy = dir.second;
        int dx = dir.first;
        int curY = y + dy;
        int curX = x + dx;
        while(board.checkSquareAvailability(curY, curX)){
            availableMoves.push_back({curY, curX});
            curY += dy;
            curX += dx;
        }
    }
    return availableMoves;
}

std::vector<std::pair<int,int>> Piece::checkCapture(Board& board){
    int y = position.first;
    int x = position.second;
    std::vector<std::pair<int,int>> availableCaptures = {};

    for(auto& dir : directions){
        int dy = dir.second;
        int dx = dir.first;
        int curY = y + dy;
        int curX = x + dx;
        while(board.isInBounds(curY, curX)){
            if(board.checkSquareAvailability(curY, curX)){
                curY += dy;
                curX += dx;
                continue;
            }
            if(board.isAvailableToCapture(color, curY, curX)){
                availableCaptures.push_back({curY, curX});
            }
            break;
        }
    }
    return availableCaptures;
}

std::string Piece::vectorToString(std::vector<std::pair<int,int>> vector){
    std::string convertedString = "";
    for (auto& element: vector){
        int x = element.first;
        int y = element.second;
        convertedString +=  std::to_string(x) + std::to_string(y) + "&";
    }
    return convertedString;

}