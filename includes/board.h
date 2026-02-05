#pragma once

#include <iostream>
#include "types.h"
class Piece;

class Board {
private:
    std::unique_ptr<Piece> board[8][8];

public:
    bool inBounds(int x, int y) {
        return x >= 0 && x <= 7 && y >= 0 && y <= 7;
    }
    Piece* getPiece(int x, int y);

    bool Board::isAvailableToCapture(ColorType color, int x, int y);
    bool checkSquareAvailability(int x, int y);

    void placePiece(std::unique_ptr<Piece> piece);

    void stateBoard();
};
