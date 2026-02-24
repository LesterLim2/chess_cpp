#include <iostream>
#include <vector>

#include "dangerMap.h"
#include "piece.h"
#include "board.h"


DangerMap::DangerMap()
    : dangerMap(8, std::vector<dangerType>(8)) {}


std::vector<std::vector<std::vector<ColorType>>>& DangerMap::getDangerMap(){
    return dangerMap;
}
void DangerMap::insertDanger(int x, int y, ColorType color){
    dangerMap[x][y].push_back(color);
}


void DangerMap::removeDanger(int x, int y, ColorType color){
    int totalThreats = (int)dangerMap[x][y].size();

    for (int i = 0 ; i < totalThreats; i++){
        ColorType currentColor = dangerMap[x][y][i];
        if (currentColor == color){
            dangerMap[x][y].erase(dangerMap[x][y].begin() + i);
            std::cout << "threat at (" << x << "," << y << ") removed" << std::endl;  
            break;
        }
    }
}

//when game starts
void DangerMap::initaliseDanger(Board& board){
    for(int y = 0; y < 8 ; y++){
        for(int x = 0 ; x < 8 ; x++){
            Piece* piece = board.getPiece(x,y);
            if (piece == nullptr) continue;
            piece->addDangerTiles(board,*this);
        }
    }
}

void DangerMap::stateDangerMap(){
    for (int y = 7; y >= 0; --y) {
        for (int x = 0; x < 8; ++x) {
            std::cout << (dangerMap[x][y].empty() ? ". " : "! ");
        }
        std::cout << "\n";
    }
}

bool DangerMap::hasDangerColor(int x, int y, ColorType c) const {
    if (x < 0 || x > 7 || y < 0 || y > 7) return false;
    for (ColorType v : dangerMap[x][y]) {
        if (v == c) return true;
    }
    return false;
}

int DangerMap::getThreatCount(int x, int y, ColorType c) const {
    if (x < 0 || x > 7 || y < 0 || y > 7) return 0;
    int count = 0;
    for (ColorType v : dangerMap[x][y]) {
        if (v == c) count++;
    }
    return count;
}

int DangerMap::getTotalThreats(int x, int y) const {
    if (x < 0 || x > 7 || y < 0 || y > 7) return 0;
    return static_cast<int>(dangerMap[x][y].size());
}
