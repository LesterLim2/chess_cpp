#pragma once

#include <iostream>
class Piece;

class Board {
private:
    std::unique_ptr<Piece> board[8][8];

public:
    bool inBounds(int x, int y) const {
        return x >= 0 && x <= 7 && y >= 0 && y <= 7;
    }

    void placePiece(std::unique_ptr<Piece> piece);

    void stateBoard();
};
