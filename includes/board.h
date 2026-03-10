#pragma once

#include <iostream>
#include <memory>
#include <array>

#include "piece.h"
class Piece;

class DangerMap;

class Player;

class Board {
private:
    std::vector<std::vector<std::unique_ptr<Piece>>> board;
protected:
    std::unique_ptr<Piece> createPiece(int x,int y,std::string color,std::string type);

public:
    Board();
    bool isInBounds(int x, int y) {
        return x >= 0 && x <= 7 && y >= 0 && y <= 7;
    }
    Piece* getPiece(int x, int y);

    bool isAvailableToCapture(ColorType color, int x, int y);
    bool checkSquareAvailability(int x, int y);

    void placePiece(std::unique_ptr<Piece>);
    void movePiece(int originalX,int originalY,int newX,int newY);

    void stateBoard();
    void removePiece(Piece* piece);

    void generateBoard();
    void backRowPlace(int row, int col);
};
