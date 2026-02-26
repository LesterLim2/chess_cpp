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


void DangerMap::removeDanger(std::string availableMoves, ColorType color, PieceType type){
std::cout << "im changing dangerBlack" <<std::endl;
    std::vector<std::vector<int>>& selectedDangerMap =  color == ColorType::White ? dangerMapWhite : dangerMapBlack;
    if(selectedDangerMap == dangerMapBlack){
        std::cout << "fucking loser" <<std::endl;
    }
    std::cout << "please die asshole";
    if (type == PieceType::Bishop || type == PieceType::Queen || type == PieceType::Rook){
        for(int i = 0; i < availableMoves.size() ; i++){
            if(availableMoves[i] == 'c'){
                continue;
            }
            if(availableMoves[i] == '&'){
                selectedDangerMap[availableMoves[i-2] - '0'][availableMoves[i-1] - '0']--;
            }
        }
    }
    else{
        bool isCapturing = false;
        for(int i = 0; i < availableMoves.size(); i++){
            if(availableMoves[i] == 'c'){
                isCapturing = true;
            }
            if(!isCapturing){
                continue;
            }
            if(isCapturing && availableMoves[i] == '&'){
                selectedDangerMap[availableMoves[i-2] - '0'][availableMoves[i-1] - '0']--;

            }
        }
    }
}

void DangerMap::addDanger(std::string availableMoves, ColorType color, PieceType type){
    std::vector<std::vector<int>>& selectedDangerMap =  color == ColorType::White ? dangerMapWhite : dangerMapBlack;
    if (type == PieceType::Bishop || type == PieceType::Queen || type == PieceType::Rook){
        for(int i = 0; i < availableMoves.size() ; i++){
            if(availableMoves[i] == 'c'){
                continue;
            }
            if(availableMoves[i] == '&'){
                selectedDangerMap[availableMoves[i-2] - '0'][availableMoves[i-1] - '0']++;
            }
        }  
    }
    else{
        bool isCapturing = false;
        for(int i = 0; i < availableMoves.size(); i++){
            if(availableMoves[i] == 'c'){
                isCapturing = true;
            }
            if(!isCapturing){
                continue;
            }
            if(isCapturing && availableMoves[i] == '&'){
                selectedDangerMap[availableMoves[i-2] - '0'][availableMoves[i-1] - '0']++;
            }
        }
    }
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

