#include <iostream>

#include "game.h"
#include "board.h"
#include "dangerMap.h"
#include "piece.h"

Game::Game(Board& b,DangerMap& d): board(b),dangerMap(d){};

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

std::string Game::getMovementString(int row, int col){
    Piece* piece = board.getPiece(row,col);
    return piece->checkPreMoves(board);
}

bool Game::validatePieceMovement(const std::string& pieceStr){
    int row = pieceStr[0]-'0';
    int col = pieceStr[1]-'0';
    if (!board.isInBounds(row, col)){
        throw std::runtime_error("Error: piece is not in bounds");
        return false;
    }
    Piece* piece = board.getPiece(row, col);
    if ((pieceStr[2] == 'o' && pieceStr[3] == 'o') && piece != nullptr){
        throw std::runtime_error("Error: front and backend mismatch - expected empty square");
        return false;
    }
    if((pieceStr[2] != 'o' && pieceStr[3] != 'o') && !isValidatedPiece(piece, row, col, charToColorType[pieceStr[3]], charToPieceType[pieceStr[2]])){
        throw std::runtime_error("Error: front and backend mismatch");
        return false;
    }
    return true;
}

bool Game::movePiece(std::string pieceString){
    std::string preMovePiece = pieceString.substr(0,4);
    std::string postMovePiece = pieceString.substr(4,4);
    

    if(!validatePieceMovement(preMovePiece) || !validatePieceMovement(postMovePiece)){
        return false;
    }

    auto isSliding = [](PieceType t){
        return t == PieceType::Bishop || t == PieceType::Rook || t == PieceType::Queen;
    };

    //remove dangerMap for piece before movement
    PieceType movingType = charToPieceType[preMovePiece[2]];
    int preRow = preMovePiece[0]-'0', preCol = preMovePiece[1]-'0';
    std::string preMoveDangerString = isSliding(movingType)
        ? getMovementString(preRow, preCol)
        : board.getPiece(preRow, preCol)->checkThreats(board);
    dangerMap.removeDanger(preMoveDangerString, charToColorType[preMovePiece[3]], movingType);

    //remove dangerMap for captured piece if tile contains a piece
    if(postMovePiece[3] != 'o' && postMovePiece[2] != 'o'){
        PieceType capturedType = charToPieceType[postMovePiece[2]];
        int capRow = postMovePiece[0]-'0', capCol = postMovePiece[1]-'0';
        std::string capturedDangerString = isSliding(capturedType)
            ? getMovementString(capRow, capCol)
            : board.getPiece(capRow, capCol)->checkThreats(board);
        dangerMap.removeDanger(capturedDangerString, charToColorType[postMovePiece[3]], capturedType);
    }

    //movement of piece + update of dangerMap corresponding to the piece new position
    board.movePiece(preMovePiece[0]-'0', preMovePiece[1]-'0',postMovePiece[0]-'0', postMovePiece[1]-'0');
    int postRow = postMovePiece[0]-'0', postCol = postMovePiece[1]-'0';
    std::string postMoveDangerString = isSliding(movingType)
        ? getMovementString(postRow, postCol)
        : board.getPiece(postRow, postCol)->checkThreats(board);
    dangerMap.addDanger(postMoveDangerString, charToColorType[preMovePiece[3]], movingType);

    std::cout << "white\n" <<std::endl;
    dangerMap.stateDangerMap(ColorType::White);
    std::cout << "black\n" <<std::endl;
    dangerMap.stateDangerMap(ColorType::Black);
    return true;
}


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

