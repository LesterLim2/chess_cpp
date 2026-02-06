#include <iostream>
#include <utility>

#include "board.h"
#include "piece.h"

using namespace std;


Board::Board() {
    for (int i = 0; i < 8; i++) {
        board.push_back(std::vector<std::unique_ptr<Piece>>(8));
    }
}

Piece* Board::getPiece(int x, int y) {
    if (!inBounds(x, y)) return nullptr;
    return board[x][y].get();
}

//used for actual piece movement in game, function is dumb to make it general for any piece to use, and thus needs 
//to be carefully managed. 
void Board::movePiece(int originalX,int originalY,int newX,int newY){
    if (!inBounds(originalX,originalY) || !inBounds(newX,newY)){
        throw runtime_error("out of bounds");
    }
    Piece* original = board[originalX][originalY].get();
    if (original == nullptr){
        throw runtime_error("original piece not found");
    }
    Piece* newPiece = board[newX][newY].get();
    if (newPiece){
        board[newX][newY] = nullptr;
    }
    board[newX][newY] = move(board[originalX][originalY]);
    board[originalX][originalY] = nullptr;
    board[newX][newY]->setPosition(newX,newY);
    return;
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

//used for game initalisation, manual placing of pieces.
void Board::placePiece(unique_ptr<Piece> piece){
    pair<int,int> position = piece->getPosition();
    int x = position.first;
    int y = position.second;
    board[x][y] = move(piece);
}

//iterates through board and prints out its elements, needs to be refactored.
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