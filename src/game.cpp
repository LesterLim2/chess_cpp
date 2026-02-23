#include <iostream>

#include "game.h"
#include "board.h"
#include "dangerMap.h"
#include "piece.h"

Game::Game(Board& b): board(b){};

std::string Game::preMove(int row, int col, ColorType color, PieceType type){
    Piece* piece = board.getPiece(row,col);

    if (piece == nullptr && color != ColorType::None && type !=PieceType::None) return "Error: Piece mismatch";

    std::string availablePreMoves = "None";
    if(piece != nullptr){
        if(!isValidatedPiece(piece,row,col,color,type)){
            return "Error: piece mismatch";
        }
        if(color != ColorType::None && type != PieceType::None){
            availablePreMoves = piece->checkPreMoves(board);
        }
    }
    return availablePreMoves;
};

bool Game::isValidatedPiece(Piece* piece, int row, int col, ColorType color, PieceType type){
    std::pair<int,int> piecePosition = piece->getPosition();
    int pieceRow = piecePosition.first;
    int pieceCol = piecePosition.second;
    return (
        pieceRow == row &&
        pieceCol == col &&
        piece->getColor() == color && 
        piece->getType() == type
    );
}

std::string Game::vectorToString(std::vector<std::pair<int,int>> vector){
    if (vector.empty()) return "None";
    std::string convertedString = "";
    for (auto& element: vector){
        int x = element.first;
        int y = element.second;
        convertedString +=  std::to_string(x) + std::to_string(y) + "&";
    }
    return convertedString;

}

