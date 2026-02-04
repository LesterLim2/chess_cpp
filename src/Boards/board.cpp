#include <iostream>
#include <utility>

#include "board.h"
#include "piece.h"

using namespace std;

void Board::placePiece(std::unique_ptr<Piece> piece) {
    pair<int,int> position = piece->getPosition();
    int x = position.first;
    int y = position.second;

    if (!inBounds(x, y)) {
        throw std::out_of_range("x and y must be between 0 and 7");
    }

    if (board[x][y]) {
        throw std::runtime_error("Space already occupied");
    }

    board[x][y] = std::move(piece);
}

void Board::stateBoard(){
    for(int i = 0; i < 8; i++){
        for (int j = 0 ; j < 8 ; j++){
            if (!board[i][j]){
                cout << "no piece exists at (" <<i << "," << j << ")" <<endl;
            }
            else{
                cout << "piece exists at" << i << j << endl;
            }
        }
    }
}