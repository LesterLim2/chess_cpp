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

    //remove dangerMap for piece before movement
    ColorType movingColor = charToColorType[preMovePiece[3]];
    PieceType movingPiece = charToPieceType[preMovePiece[2]];
    int preRow = preMovePiece[0]-'0', preCol = preMovePiece[1]-'0';
    dangerMap.removeDanger(getMovementString(preRow, preCol), charToColorType[preMovePiece[3]]);

    //remove dangerMap for captured piece if tile contains a piece
    if(postMovePiece[3] != 'o' && postMovePiece[2] != 'o'){
        ColorType capturedColor = charToColorType[postMovePiece[3]];
        int capRow = postMovePiece[0]-'0', capCol = postMovePiece[1]-'0';
        dangerMap.removeDanger(getMovementString(capRow, capCol), capturedColor);
    }

    //movement of piece + update of dangerMap corresponding to the piece new position
    board.movePiece(preMovePiece[0]-'0', preMovePiece[1]-'0',postMovePiece[0]-'0', postMovePiece[1]-'0');
    int postRow = postMovePiece[0]-'0', postCol = postMovePiece[1]-'0';
    dangerMap.addDanger(getMovementString(postRow, postCol), movingColor);
    
    dangerMap.updateBlockPieces(board,preRow,preCol,movingColor,postRow,postCol);
    dangerMap.updateBlockPieces(board,postRow,postCol,movingColor);
    
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
        specialEvents += "C";
        specialEvents += getPossibleunCheckMoves(opponentColor);
    }
    return specialEvents;
}

//wtffff this is going to be a n^4 solution
std::string Game::getPossibleunCheckMoves(ColorType opponentColor,std::pair<int,int> kingCoordinates){
    std::string possibleunCheckMoves = "";
    for(int i = 0; i < 8 ; i++){
        for(int j = 0; j < 8; j++){
            Piece* currentPiece = board.getPiece(i,j);
            if(currentPiece == nullptr){
                continue;
            }
            if(currentPiece->getColor() == opponentColor ){
                std::string simulateUncheckResults = simulateUncheckMoves(currentPiece,kingCoordinates);
                if(simulateUncheckResults.length() > 0){
                    possibleunCheckMoves += j + '0';
                    possibleunCheckMoves += i + '0';
                    possibleunCheckMoves += 'p'; //p for piece, meant to denote which moves originate from what piece
                    possibleunCheckMoves += simulateUncheckResults;
                } 
            }
        }
    }
}

std::string Game::simulateUncheckMoves(Piece* currentPiece,std::pair<int,int> kingCoordinates){
    std::string availanlePieceMoves = currentPiece->checkPreMoves(board);
    DangerMap dangerMapCopy = dangerMap;
    std::cout << "im such a fat fucking chud" << std::endl;
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

