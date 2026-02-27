#include <iostream>
#include <vector>

#include "dangerMap.h"
#include "piece.h"
#include "board.h"
#include "types.h"


DangerMap::DangerMap()
    : dangerMapWhite(8,std::vector<int>(8,0)),dangerMapBlack(8,std::vector<int>(8,0)){
        initaliseDanger();
    }


std::vector<std::vector<int>> DangerMap::getDangerMap(ColorType color){
    return color == ColorType::White ? dangerMapWhite : dangerMapBlack;
}
void DangerMap::insertDanger(int x, int y,ColorType color){
    color == ColorType::White ? dangerMapWhite[x][y]++ : dangerMapBlack[x][y]++;
}


int DangerMap::getDangerCount(int row,int col,ColorType color){
    std::vector<std::vector<int>>& selectedDangerMap =  color == ColorType::White ? dangerMapWhite : dangerMapBlack;
    return selectedDangerMap[row][col];
}

void DangerMap::removeDanger(std::string availableMoves, ColorType color){
    std::vector<std::vector<int>>& selectedDangerMap =  color == ColorType::White ? dangerMapWhite : dangerMapBlack;
    std::cout << "im in remove danger" <<std::endl;
    for(int i = 0; i < availableMoves.size() ; i++){
        if(availableMoves[i] == 'c'){
            continue;
        }
        if(availableMoves[i] == '&'){
            selectedDangerMap[availableMoves[i-2] - '0'][availableMoves[i-1] - '0']--;
        }
    }
    color == ColorType::White ? stateDangerMap(ColorType::White) : stateDangerMap(ColorType::Black);
}

void DangerMap::addDanger(std::string availableMoves, ColorType color){
    std::vector<std::vector<int>>& selectedDangerMap =  color == ColorType::White ? dangerMapWhite : dangerMapBlack;
    for(int i = 0; i < availableMoves.size() ; i++){
        if(availableMoves[i] == 'c'){
            continue;
        }
        if(availableMoves[i] == '&'){
            selectedDangerMap[availableMoves[i-2] - '0'][availableMoves[i-1] - '0']++;
        }
    }
    color == ColorType::White ? stateDangerMap(ColorType::White) : stateDangerMap(ColorType::Black);
}

//when game starts
void DangerMap::initaliseDanger(){
    for(int j= 0; j < 8;j++){
        dangerMapWhite[2][j] = initialDanger[j];
    }
    for(int j= 0; j < 8;j++){
        dangerMapBlack[5][j] = initialDanger[j];
    }
    std::cout << "stating initalised dangerMap, white then black" << std::endl;

    std::cout << "white\n" <<std::endl;
    stateDangerMap(ColorType::White);
    std::cout << "black\n" <<std::endl;
    stateDangerMap(ColorType::Black);

}

void DangerMap::updateBlockPieces(Board& board, int originalRow, int originalCol, ColorType color, int skipRow, int skipCol){
    for(auto& dir:dangerDirections){
        int dy = dir.first;
        int dx = dir.second;
        int row = originalRow;
        int col = originalCol;
        row += dx;
        col += dy;
        std::vector<std::pair<int,int>> updatedTiles =  {{originalRow,originalCol}};
        while(board.isInBounds(row,col)){

            Piece* piece = board.getPiece(row,col);

            if(piece == nullptr){
                updatedTiles.push_back({row,col});
                row += dx;
                col += dy;
                continue;
            }
            if(row == skipRow && col == skipCol){
                break;
            }
            PieceType type  = piece->getType();
            std::vector<std::pair<int,int>> pieceDirections = piece->getDirections();
            ColorType pieceColor = piece->getColor();
            bool isAffected = std::find(pieceDirections.begin(),pieceDirections.end(),dir) != pieceDirections.end();
            
            if (piece->getIsSliding() && isAffected){
                std::vector<std::vector<int>>& selectedDangerMap = pieceColor == ColorType::White ? dangerMapWhite : dangerMapBlack;
                for(auto& tile: updatedTiles){
                    selectedDangerMap[tile.first][tile.second]++;
                }
                break;
            }
            else{
                break;
            }
            row += dx;
            col += dy;
        }
    }
}

void DangerMap::stateDangerMap(ColorType color){
    std::vector<std::vector<int>> selectedDangerMap = color == ColorType::White ? dangerMapWhite : dangerMapBlack;
    std::cout << "  0 1 2 3 4 5 6 7\n";
    for (int y = 7; y >= 0; --y) {
        std::cout << y << " ";
        for (int x = 0; x < 8; ++x) {
            std::cout << selectedDangerMap[y][x] << " ";
        }
        std::cout << "\n";
    }
}

