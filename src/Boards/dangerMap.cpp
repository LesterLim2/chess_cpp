#include <iostream>
#include <vector>

#include "dangerMap.h"


DangerMap::DangerMap()
    : dangerMap(8, std::vector<dangerType>(8)) {}


void DangerMap::insertDanger(Piece* piece,Board& board) {
    piece->addDangerTiles(board);
    std::vector<std::pair<int,int>> dangerTiles = piece->getDangerTiles(); 

    for (auto& tile : dangerTiles){
        int x = tile.first;
        int y = tile.second;

        dangerMap[x][y].push_back(piece->getColor());
    }
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