#include <iostream>
#include <utility>
#include <stdexcept>

#include "board.h"
#include "pawn.h"
#include "bishop.h"
#include "types.h"


Board::Board() {
    for (int i = 0; i < 8; i++) {
        board.push_back(std::vector<std::unique_ptr<Piece>>(8));
    }
}


Piece* Board::getPiece(int x, int y) {
    if (!isInBounds(x, y) || board[x][y] == nullptr) return nullptr;
    return board[x][y].get();
}


void Board::movePiece(int originalX, int originalY, int newX, int newY) {
    if (!isInBounds(originalX, originalY) || !isInBounds(newX, newY)) {
        throw std::runtime_error("out of bounds");
    }
    Piece* original = getPiece(originalX,originalY);
    if (original == nullptr) {
        throw std::runtime_error("original piece not found");
    }

    board[newX][newY] = std::move(board[originalX][originalY]);
    board[originalX][originalY].reset();
    board[newX][newY]->setPosition(newX, newY);
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
                    std::cout<< "Row " << i << "generated" << std::endl;
                    break;
                case(1):
                    placePiece(std::make_unique<Pawn>(ColorType::White, std::pair<int,int> {i,j}));
                    std::cout<< "Row " << i << "generated" << std::endl;
                    break;
                case(6):
                    placePiece(std::make_unique<Pawn>(ColorType::Black, std::pair<int,int> {i,j}));
                    std::cout<< "Row " << i << "generated" << std::endl;
                    break;
                case(7):
                    backRowPlace(i,j);
                    std::cout<< "Row " << i << "generated" << std::endl;
                    break;
                default:
                    std::cout << "Row " << i << "passed" << std::endl;
            }
        }
    }
    stateBoard();
}

void Board::backRowPlace(int row, int col){
    ColorType color = row == 0 ? ColorType::White : ColorType::Black;
    std::pair<int,int> pos = std::make_pair(row, col);

    switch(col){
        case 0: // place Rook at pos with color
        case 7: // place Rook at pos with color
            break;
        case 1: // place Knight at pos with color
        case 6: // place Knight at pos with color
            break;
        case 2: // place Bishop at pos with color
            placePiece(std::make_unique<Bishop>(color, std::make_pair(row,col)));
            break;
        case 5: // place Bishop at pos with color
            placePiece(std::make_unique<Bishop>(color, std::make_pair(row,col)));
            break;
        case 3: // place Queen at pos with color
            break;
        case 4: // place King at pos with color
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
