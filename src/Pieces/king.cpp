#include <iostream>

#include "king.h"
#include "board.h"
#include "dangerMap.h"

King::King(ColorType color, std::pair<int,int> position)
    : Piece(color,PieceType::King,position){
        directions = {{1,0},{-1,0},{0,1},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1}};
        danger = directions;
    }
std::string King::checkMovement(Board& board){
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

std::string King::checkMovement(Board& board, DangerMap& dangerMap){
    std::string result = checkMovement(board);
    if(!hasMoved){
        std::string availableCastles = "";
        result += checkCastle(board, dangerMap, availableCastles);
    }
    return result != "c" ? result : "None";
}

std::vector<std::pair<int,int>> King::getDanger(Board& board){
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


std::string King::checkCastle(Board& board, DangerMap& dangerMap, std::string& availableCastles){
    int castleRow = color == ColorType::White ? 0 : 7;
    for(int castleCol : castleCols){
       
        Piece* piece = board.getPiece(castleRow,castleCol);
        if(piece->getType() != PieceType::Rook && piece->getHasMoved()){
            continue;
        }
        auto [row,col] = position; 
        if(row != castleRow){
            std::cout << "error, kings original position does not exist at the original row" <<std::endl;
            return "";
        }

        ColorType checkColor = color == ColorType::White ? ColorType::Black : ColorType::White;
        int dir = col > castleCol ? -1 : 1;
        int curCol = col + dir;
        int curRow = castleRow;
        bool canCastle = true;
        while(std::abs(curCol - castleCol) > 0){
            if(board.getPiece(curRow,curCol) != nullptr || dangerMap.getDangerCount(curRow,curCol,checkColor) > 0){
                canCastle = false;
                break;
            }
            curCol += dir;
        }

        if(!canCastle) continue;
       
        //all checks succeded append king and rook coordinates
        availableCastles += "o";
        //append kings coordinates
        availableCastles += row + '0';
        availableCastles += col + '0';        
        //append rook coordinates
        availableCastles += castleRow + '0';
        availableCastles += castleCol + '0';
        availableCastles += '&';
        std::cout << "@king.cpp/checkCastle" << " king is able to castle moveString == " << availableCastles << std::endl;
    }

    return availableCastles;
}