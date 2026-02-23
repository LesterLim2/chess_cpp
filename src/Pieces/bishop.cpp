#include <iostream>
#include "utility"
#include "bishop.h"
#include "board.h"


Bishop::Bishop(ColorType color, std::pair<int,int> position)
    : Piece(color, PieceType::Bishop, position, {}) {}


// std::vector<std::pair<int,int>> Bishop::checkPreMoves(Board& board){
//     std::vector<std::pair<int,int>> availablePreMoves = {};
//     checkMovement(board,availablePreMoves);
//     checkCapture(board,availablePreMoves);
// }

// void Bishop::checkCapture(Board& board,std::vector<std::pair<int,int>>& availablePreMoves){

// };
// void Bishop::checkMovement(Board& board,std::vector<std::pair<int,int>>& availablePreMoves){
//     auto& [y,x] = position;

//     for(auto& dir: directions){

//     }
// };
void Bishop::addDangerTiles(Board& board){
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
                dangerTiles.push_back({x,y});
            }
            else if (piece->getColor() != color){
                if(piece->getType() == PieceType::King){
                    //check flag (to be implemented)
                }
                dangerTiles.push_back({x,y});
            }
            else{
                break;
            }
            x += xDir; 
            y += yDir;
        }
    }
}

