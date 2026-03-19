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
    bool hasMoved = false;
    bool isSliding = false;
    int firstMovement = 0;

public:
    Piece(ColorType color = ColorType::None,
          PieceType type = PieceType::None,
          std::pair<int,int> position = {-1,-1});

    Piece(std::pair<int,int> position);   
    virtual ~Piece() = default;

    virtual void stateType();
    PieceType getType();

    std::pair<int,int> getPosition();
    void statePosition();
    void setPosition(int x,int y);

    ColorType Piece::getColor();

    virtual std::vector<std::pair<int,int>> getDanger(Board& board);
    std::vector<std::pair<int,int>> getDangerTiles();
    
    virtual std::string checkMovement(Board& board);
    std::string checkThreats(Board& board);

    bool getHasMoved();
    void setHasMoved(bool hasMoved);
    void setFirstMovement(int firstMovement);
    int getFirstMovement();
    bool getIsSliding();

    std::vector<std::pair<int,int>> getDirections();
};