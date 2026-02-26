#pragma once

#include <iostream>
#include <vector>

class Board;
class Piece;
enum class ColorType;
enum class PieceType;

class DangerMap{
private:
    std::vector<std::vector<int>> dangerMapWhite;
    std::vector<std::vector<int>> dangerMapBlack;
    ColorType color;
    std::vector<int> initialDanger = {2,2,3,2,2,3,2,2};
public:

    DangerMap();
    std::vector<std::vector<int>> getDangerMap(ColorType color);
    void insertDanger(int x, int y,ColorType color);

    void removeDanger(std::string availableMoves, ColorType color, PieceType type);
    void addDanger(std::string availableMoves, ColorType color, PieceType type);
    
    void updateDanger(Piece* piece,int newX,int newY);

    void stateDangerMap(ColorType color);
    void initaliseDanger();
};
