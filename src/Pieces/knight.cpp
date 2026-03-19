#include <iostream>

#include "knight.h"
#include "board.h"

Knight::Knight(ColorType color, std::pair<int,int> position)
    : Piece(color,PieceType::Knight,position){
    directions = {{2,1},{2,-1},{-2,1},{-2,-1},{1,2},{1,-2},{-1,2},{-1,-2}};
    danger = directions;
};

std::string Knight::checkMovement(Board& board){
    int originalX = position.second;
    int originalY = position.first;
    std::string availableMoves = "";
    std::string availableCaptures = "";

    for (auto& dir : directions){
        int dy = dir.second;
        int dx = dir.first;
        int curY = originalY + dy;
        int curX = originalX + dx;
        if (!board.isInBounds(curY, curX)) continue;
        if (board.getPiece(curY,curX) == nullptr){
            availableMoves+= curY + '0';
            availableMoves += curX + '0';
            availableMoves += '&';
        }
        if (board.isAvailableToCapture(color, curY, curX)){
            availableCaptures += curY + '0';
            availableCaptures += curX + '0';
            availableCaptures += '&';
        }
    }

    std::string result = availableMoves + "c" + availableCaptures;
    return result != "c" ? result : "None";
}


std::vector<std::pair<int,int>> Knight::getDanger(Board& board){
    int row = position.first;
    int col = position.second;
    std::vector<std::pair<int,int>> dangerTiles = {};
    for(auto& d : directions){
        int curRow = row + d.second;
        int curCol = col + d.first;
        if(board.isInBounds(curRow,curCol)){
            dangerTiles.push_back({curRow,curCol});
        }
    }
    return dangerTiles;
}