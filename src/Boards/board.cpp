#include <iostream>
#include <utility>

#include "board.h"
#include "piece.h"

using namespace std;

Piece* Board::getPiece(int x, int y) {
    if (!inBounds(x, y)) return nullptr;
    return board[x][y].get();
}

bool Board::checkSquareAvailability(int x, int y) {
    if (!inBounds(x, y)) {
        return false;
    }
    return board[x][y] == nullptr;
}


// scans the x and y coordinates for opposing pieces (must be in bounds and must be of the opposite color, and returns a boolean value)
bool Board::isAvailableToCapture(ColorType color, int x, int y) {
    if (!inBounds(x, y) || board[x][y] == nullptr) {
        return false;
    }

    Piece* capturee = board[x][y].get();

    if (capturee->getColor() == color) {
        return false;
    }

    return true;
}

void Board::placePiece(unique_ptr<Piece> piece){
    pair<int,int> position = piece->getPosition();
    int x = position.first;
    int y = position.second;
    board[x][y] = move(piece);
}

void Board::stateBoard(){
    for(int i = 0; i < 8; i++){
        for (int j = 0 ; j < 8 ; j++){
            if (!board[i][j]){
                cout << "no piece exists at (" <<i << "," << j << ")" <<endl;
            }
            else{
                Piece* p = board[i][j].get();
               
                p->stateType();
                cout  << "and i am at" << "(" << i << "," << j << ")" <<  endl;
            }
        }
    }
}