#include <iostream>
#include <utility>
#include <stdexcept>

#include "board.h"


Board::Board() {
    for (int i = 0; i < 8; i++) {
        board.push_back(std::vector<std::unique_ptr<Piece>>(8));
    }
}


Piece* Board::getPiece(int x, int y) {
    if (!isInBounds(x, y)) return nullptr;
    return board[x][y].get();
}


void Board::movePiece(int originalX, int originalY, int newX, int newY,DangerMap& dangerMap) {
    if (!isInBounds(originalX, originalY) || !isInBounds(newX, newY)) {
        throw std::runtime_error("out of bounds");
    }
    Piece* original = getPiece(originalX,originalY);
    if (original == nullptr) {
        throw std::runtime_error("original piece not found");
    }
    ColorType color = original->getColor();

    for (auto& tile : original->getDangerTiles()){
        int curX = tile.first;
        int curY = tile.second;
        dangerMap.removeDanger(curX,curY,color);
    }
    original->setDangerTiles({});

    board[newX][newY] = std::move(board[originalX][originalY]);
    board[originalX][originalY].reset();
    board[newX][newY]->setPosition(newX, newY);

    dangerMap.insertDanger(board[newX][newY].get(),*this);
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
    return true;
}


void Board::placePiece(Piece* piece,DangerMap& dangerMap) {
    std::pair<int, int> position = piece->getPosition();
    int x = position.first;
    int y = position.second;
    if (!isInBounds(x,y)) throw std::runtime_error("piece not in bounds");
    board[x][y] = std::unique_ptr<Piece>(piece);
    
    dangerMap.insertDanger(piece, *this);
}

//when a piece is captured run this code
void Board::removePiece(Piece* piece, DangerMap& dangerMap){
    std::pair<int, int> position = piece->getPosition();
    int x = position.first;
    int y = position.second;

    if (!isInBounds(x,y)) throw std::runtime_error("piece not in bounds");

    ColorType color = piece->getColor();
    std::vector<std::pair<int,int>> dangerTiles = piece->getDangerTiles();

    for (auto& tile : dangerTiles){
        int curX = tile.first;
        int curY = tile.second;
        dangerMap.removeDanger(curX,curY,color);
    }
    board[x][y].reset();
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
