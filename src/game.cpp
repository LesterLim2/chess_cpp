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
    return piece->getIsSliding() ? piece->checkPreMoves(board) : piece->checkThreats(board);
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



std::string Game::movePiece(std::string pieceString){
    std::string preMovePiece = pieceString.substr(0,4);
    std::string postMovePiece = pieceString.substr(4,4);
    

    if(!validatePieceMovement(preMovePiece) || !validatePieceMovement(postMovePiece)){
        return "Invalid";
    }

    ColorType movingColor = charToColorType[preMovePiece[3]];
    PieceType movingPiece = charToPieceType[preMovePiece[2]];
    int preRow = preMovePiece[0]-'0', preCol = preMovePiece[1]-'0';
    int postRow = postMovePiece[0]-'0', postCol = postMovePiece[1]-'0';

    //remove dangerMap for piece before movement
    dangerMap.removeDanger(getMovementString(preRow, preCol), movingColor);

    //remove dangerMap for captured piece if tile contains a piece
    if(postMovePiece[3] != 'o' && postMovePiece[2] != 'o'){
        ColorType capturedColor = charToColorType[postMovePiece[3]];
        dangerMap.removeDanger(getMovementString(postRow, postCol), capturedColor);
    }

    //movement of piece + update of dangerMap corresponding to the piece new position
    board.movePiece(preRow, preCol, postRow, postCol);
    dangerMap.addDanger(getMovementString(postRow, postCol), movingColor);

    
    dangerMap.updateBlockPieces(board,preRow,preCol,movingColor,postRow,postCol);
    std::cout << "dangerMap after updating blocked pieces" << std::endl;
    dangerMap.stateDangerMap(ColorType::Black);
    
    std::pair<int,int>& kingCoordinates = movingColor == ColorType::White ? whiteKingCoordinates : blackKingCoordinates;
    if(movingPiece == PieceType::King){
        kingCoordinates = {postRow,postCol}; 
    }

    std::cout << "white\n" <<std::endl;
    dangerMap.stateDangerMap(ColorType::White);
    std::cout << "black\n" <<std::endl;
    dangerMap.stateDangerMap(ColorType::Black);


    std::string specialEvents = "";
    if(movingPiece == PieceType::Pawn){
        if(checkPromotion(movingColor,postRow)){
            specialEvents += "p";
        }
    }

    ColorType opponentColor = movingColor == ColorType::White ? ColorType::Black : ColorType::White;
    if(dangerMap.getDangerCount(kingCoordinates.second,kingCoordinates.first,opponentColor) > 0){
        specialEvents += "c";
        specialEvents += getPossibleunCheckMoves(opponentColor, kingCoordinates, {postRow, postCol}, movingPiece);
    }
    return specialEvents;
}

//wtffff this is going to be a n^4 solution
std::string Game::getPossibleunCheckMoves(ColorType opponentColor, std::pair<int,int> kingCoordinates, std::pair<int,int> checkerCoordinates, PieceType checkerType){
    std::string possibleunCheckMoves = "";
    for(int i = 0; i < 8 ; i++){
        for(int j = 0; j < 8; j++){
            Piece* currentPiece = board.getPiece(i,j);
            if(currentPiece == nullptr){
                continue;
            }
            if(currentPiece->getColor() == opponentColor ){
                std::string simulateUncheckResults = simulateUncheckMoves(currentPiece, kingCoordinates, checkerCoordinates, checkerType);
                if(simulateUncheckResults.length() > 0){
                    possibleunCheckMoves += j + '0';
                    possibleunCheckMoves += i + '0';
                    possibleunCheckMoves += 'p'; //p for piece, meant to denote which moves originate from what piece
                    possibleunCheckMoves += simulateUncheckResults;
                }
            }
        }
    }
    return "";
}

std::string Game::simulateUncheckMoves(Piece* currentPiece, std::pair<int,int> kingCoordinates, std::pair<int,int> checkerCoordinates, PieceType checkerType){
    std::string availablePieceMoves = currentPiece->checkPreMoves(board);
    DangerMap dangerMapCopy = dangerMap;
    return "";
}

bool Game::checkPromotion(ColorType movingColor,int postRow){
    std::cout << postRow << std::endl;
    int promotionTile = movingColor == ColorType::White ? 6 : 0;
    if (promotionTile == postRow){
        return true;
    }
    return false;
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

