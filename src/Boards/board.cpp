#include <iostream>
#include <utility>
#include <stdexcept>

#include "board.h"
#include "pawn.h"
#include "bishop.h"
#include "rook.h"
#include "queen.h"
#include "knight.h"
#include "king.h"
#include "types.h"


Board::Board() {
    for (int i = 0; i < 8; i++) {
        board.push_back(std::vector<std::unique_ptr<Piece>>(8));
    }
    generateBoard();
}


Piece* Board::getPiece(int x, int y) {
    if (!isInBounds(x, y) || board[x][y] == nullptr) return nullptr;
    return board[x][y].get();
}


void Board::movePiece(int originalX, int originalY, int newX, int newY) {
    Piece* piece = getPiece(originalX,originalY);
    if (!piece->getHasMoved()){
        piece->setHasMoved(true);
    }
    board[newX][newY] = std::move(board[originalX][originalY]);
    board[newX][newY]->setPosition(newX, newY);
    stateBoard();
}


bool Board::checkSquareAvailability(int x, int y) {
    if (!isInBounds(x, y)) {
        return false;
    }
    return !board[x][y];
}


bool Board::isAvailableToCapture(ColorType color, int x, int y) {
    if (!isInBounds(x, y) || board[x][y] == nullptr) {
        return false;
    }
    Piece* capturee = board[x][y].get();
    if (capturee->getColor() == color) {
        return false;
    }
    if(capturee->getType() == PieceType::King){
        return false;
    }
    return true;
}

//used on game start only
void Board::placePiece(std::unique_ptr<Piece> piece) {
    std::pair<int, int> position = piece->getPosition();
    int x = position.first;
    int y = position.second;
    if (!isInBounds(x,y)) throw std::runtime_error("piece not in bounds");
    board[x][y] = std::move(piece);
}

//when a piece is captured run this code
void Board::removePiece(Piece* piece){
    std::pair<int, int> position = piece->getPosition();
    int x = position.first;
    int y = position.second;

    if (!isInBounds(x,y)) throw std::runtime_error("piece not in bounds");

    board[x][y].reset();
}

//called when the game begins
void Board::generateBoard(){
    for(int i = 0; i < 8; i++){
        for(int j = 0 ; j < 8;j++){
            switch(i){
                case(0):
                    backRowPlace(i,j);
                    break;
                case(1):
                    placePiece(std::make_unique<Pawn>(ColorType::White, std::pair<int,int> {i,j}));
                    break;
                case(6):
                    placePiece(std::make_unique<Pawn>(ColorType::Black, std::pair<int,int> {i,j}));
                    break;
                case(7):
                    backRowPlace(i,j);
                    break;
            };
        }
    }
    std::cout << "board generated" << std::endl;
    stateBoard();
}

void Board::backRowPlace(int row, int col){
    ColorType color = row == 0 ? ColorType::White : ColorType::Black;
    std::pair<int,int> pos = std::make_pair(row, col);

    switch(col){
        case 0: 
            placePiece(std::make_unique<Rook>(color, std::make_pair(row,col)));
            break;
        case 7: 
            placePiece(std::make_unique<Rook>(color, std::make_pair(row,col)));
            break;
        case 1:
            placePiece(std::make_unique<Knight>(color,std::make_pair(row,col)));
            break;
        case 6:
            placePiece(std::make_unique<Knight>(color,std::make_pair(row,col)));
            break;
        case 2: 
            placePiece(std::make_unique<Bishop>(color, std::make_pair(row,col)));
            break;
        case 5: 
            placePiece(std::make_unique<Bishop>(color, std::make_pair(row,col)));
            break;
        case 3:
            placePiece(std::make_unique<Queen>(color,std::make_pair(row,col)));
            break;
        case 4:
            placePiece(std::make_unique<King>(color,std::make_pair(row,col)));
            break;
    }
}
void Board::stateBoard() {
    for (int i = 7; i >= 0; i--) {
        for (int j = 0; j < 8; j++) {
            if (!board[i][j]) {
                std::cout << ".";
            } else {
                std::cout << "p";
            }
        }
        std::cout << std::endl;
    }
}
