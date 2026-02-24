#pragma once
#include "types.h"
#include "dangerMap.h"
#include <utility>
#include <vector>

class Board; 
class DangerMap;
class Piece {
protected:
    ColorType color;
    PieceType type;
    std::pair<int,int> position;
    std::vector<std::pair<int,int>> danger;
    std::vector<std::pair<int,int>> directions;
    std::vector<std::pair<int,int>> dangerTiles = {};

public:
    Piece(ColorType color = ColorType::None,
          PieceType type = PieceType::None,
          std::pair<int,int> position = {-1,-1});

    Piece(std::pair<int,int> position);   // no default needed
    virtual ~Piece() = default;

    virtual void stateType();
    PieceType getType();

    std::pair<int,int> getPosition();
    void statePosition();
    void setPosition(int x,int y);

    ColorType Piece::getColor();

    virtual std::vector<std::pair<int,int>> getDanger();

    virtual void addDangerTiles(Board& board,DangerMap& dangerMap);
    void setDangerTiles(std::vector<std::pair<int,int>> tiles);
    std::vector<std::pair<int,int>> getDangerTiles();
    void removeDangerTiles(DangerMap& dangerMap);
    
    virtual std::string checkPreMoves(Board& board);
    virtual std::vector<std::pair<int,int>> checkMovement(Board& board);
    virtual std::vector<std::pair<int,int>> checkCapture(Board& board);

    std::string vectorToString(std::vector<std::pair<int,int>> vector);
};
