#include <iostream>

#include "game.h"

Game::Game(Board& b,DangerMap& d): board(b),dangerMap(d){
    playerWhite = std::make_unique<Player>(ColorType::White);
    playerBlack = std::make_unique<Player>(ColorType::Black);
};

std::string Game::preMove(int row, int col, ColorType color, PieceType type){
    Piece* piece = board.getPiece(row,col);
    if (piece == nullptr && color != ColorType::None && type !=PieceType::None) return "Error: Piece mismatch";
    std::string availablePreMoves = "None";
    if(piece != nullptr){
        if(!isValidatedPiece(piece,row,col,color,type)){
            return "Error: piece mismatch";
        }
        if(color != ColorType::None && type != PieceType::None){
            availablePreMoves = piece->checkMovement(board);
            if(type == PieceType::Pawn && row == (color == ColorType::White ? 4 : 3)){
                availablePreMoves += checkEnPassant(row, col, color);
            }
        }
    }
    return availablePreMoves;
};

std::string Game::getMovementString(int row, int col){
    Piece* piece = board.getPiece(row,col);
    if(piece == nullptr) return "None";
    return piece->checkMovement(board);
}
 

PieceData Game::parsePieceString(const std::string& s){
    return { s[0]-'0', s[1]-'0', charToPieceType[s[2]], charToColorType[s[3]] };
}


std::string Game::movePiece(std::string pieceString){
    std::string preMovePiece = pieceString.substr(0,4);
    std::string postMovePiece = pieceString.substr(4,4);


    PieceData pre = parsePieceString(preMovePiece);
    PieceData post = parsePieceString(postMovePiece);

    try{
        validatePieceMovement(pre);
        validatePieceMovement(post);
    } catch(const std::runtime_error& e){
        std::cout << "movePiece validation error: " << e.what() << std::endl;
        return "Invalid";
    }

    ColorType movingColor = pre.color;
    PieceType movingPiece = pre.type;
    int preRow = pre.row, preCol = pre.col;
    int postRow = post.row, postCol = post.col;

    //remove dangerMap for captured piece if tile contains a piece
    if(postMovePiece[3] != 'o' && postMovePiece[2] != 'o'){
        ColorType capturedColor = charToColorType[postMovePiece[3]];
        dangerMap.removeDanger(getMovementString(postRow, postCol), capturedColor);
    }

    //remove dangerMap for piece before movement
    dangerMap.removeDanger(getMovementString(preRow, preCol), movingColor);

    //movement of piece + update of dangerMap corresponding to the piece new position
    board.movePiece(preRow, preCol, postRow, postCol);
    dangerMap.addDanger(getMovementString(postRow, postCol), movingColor);

    std::pair<int,int>& kingCoordinates = movingColor == ColorType::White ? whiteKingCoordinates : blackKingCoordinates;
    if(movingPiece == PieceType::King){
        kingCoordinates = {postRow,postCol}; 
    }
    
    updateBlockPieces(preRow,preCol,movingColor,postRow,postCol);
    std::string specialEvents = "";

    if(movingPiece == PieceType::Pawn){
        if(std::abs(preRow - postRow) == 2){
            Piece*& target = movingColor == ColorType::White ? enPassantTargetWhite : enPassantTargetBlack;
            int& counter = movingColor == ColorType::White ? enPassantCounterWhite : enPassantCounterBlack;
            target  = board.getPiece(postRow, postCol);
            counter = 1;
        }
    }
    specialEvents += handleCheck({postRow,postCol},movingPiece);   
    incrementTurn();
    return specialEvents;
}

std::string Game::pawnPromotion(const std::string& promotionPiece){
    PieceData promotedPieceData = parsePieceString(promotionPiece);
    
    try{
        validatePawnPromotion(promotedPieceData);
    }
    catch (const std::exception& e){
        std::cerr << "Exception occured: " << e.what() << std::endl;
        return "invalidated";
    }
    board.removePiece(board.getPiece(promotedPieceData.row,promotedPieceData.col));
    board.placePiece(board.createPromotionPiece(promotedPieceData.type, promotedPieceData.color, {promotedPieceData.row, promotedPieceData.col}));
    return "validated";
}



void Game::updateBlockPieces(int preRow,int preCol,ColorType movingColor,int postRow,int postCol){
    dangerMap.updateBlockPieces(board,preRow,preCol,movingColor,postRow,postCol);
    std::cout << "dangerMap after updating blocked pieces" << std::endl;
    dangerMap.stateDangerMap(ColorType::Black);

    std::cout << "white\n" <<std::endl;
    dangerMap.stateDangerMap(ColorType::White);
    std::cout << "black\n" <<std::endl;
    dangerMap.stateDangerMap(ColorType::Black);
}

std::string Game::handleCheck(std::pair<int,int> checkerCoordinates,PieceType checkerType){
    std::string checkString = "";
    ColorType opponentColor = currentTurn == ColorType::White ? ColorType::Black : ColorType::White;
    std::pair<int,int>  oppKingCords = opponentColor == ColorType::White ? whiteKingCoordinates : blackKingCoordinates;
    if(dangerMap.getDangerCount(oppKingCords.second,oppKingCords.first,opponentColor) > 0){
        checkString += getPossibleunCheckMoves(opponentColor, oppKingCords, checkerCoordinates, checkerType);
        checkString += "c";
    }
    return checkString;
}


std::string Game::checkEnPassant(int row, int col, ColorType color){
    // The opponent's pawn is the en passant target for the current color
    Piece* target = color == ColorType::White ? enPassantTargetBlack : enPassantTargetWhite;
    if(target == nullptr) return "";

    std::pair<int,int> targetPos = target->getPosition();
    std::cout << "@checkEnPassant" << "target position = " << targetPos.second - '0' << targetPos.first - '0';

    if(targetPos.first != row || std::abs(col - targetPos.second) != 1) return "";
    
    int dir = color == ColorType::White ? 1 : -1;
    std::string result = "n";
    result += (row + dir) + '0';
    result += targetPos.second + '0';
    result += '&';
    return result;
}

void Game::incrementTurn(){
    if(enPassantCounterWhite > 0){
        enPassantCounterWhite--;
    } else {
        enPassantTargetWhite = nullptr;
    }
    if(enPassantCounterBlack > 0){
        enPassantCounterBlack--;
    } else {
        enPassantTargetBlack = nullptr;
    }
    currentTurn = currentTurn == ColorType::White ? ColorType::Black : ColorType::White;
    if(currentTurn == ColorType::White) turn++;
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
                std::string possibleUncheckMoves = simulateUncheckMoves(currentPiece, kingCoordinates, checkerCoordinates, checkerType);
                if(possibleUncheckMoves.length() > 0){
                    possibleunCheckMoves += j - '0';
                    possibleunCheckMoves += i - '0';
                    possibleunCheckMoves += 'e'; 
                    possibleunCheckMoves += possibleUncheckMoves;
                }
            }
        }
    }
    return "";
}

std::string Game::simulateUncheckMoves(Piece* currentPiece, std::pair<int,int> kingCoordinates, std::pair<int,int> checkerCoordinates, PieceType checkerType){
    std::string availablePieceMoves = currentPiece->checkMovement(board);
    std::cout << "im at simulateUncheckMoves" << availablePieceMoves << std::endl;
    return "";
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

bool Game::validatePieceMovement(const PieceData& pieceData){
    if (!board.isInBounds(pieceData.row, pieceData.col)){
        throw std::runtime_error("Error: piece is not in bounds");
    }
    Piece* piece = board.getPiece(pieceData.row, pieceData.col);
    if ((pieceData.type == PieceType::None && pieceData.color == ColorType::None) && piece != nullptr){
        throw std::runtime_error("Error: front and backend mismatch - expected empty square");
    }
    if((pieceData.type != PieceType::None && pieceData.color != ColorType::None) && !isValidatedPiece(piece, pieceData.row, pieceData.col, pieceData.color, pieceData.type)){
        throw std::runtime_error("Error: front and backend mismatch");
    }
    return true;
}

bool Game::validatePawnPromotion(const PieceData& pieceData){
    if (!board.isInBounds(pieceData.row, pieceData.col)){
    throw std::runtime_error("Error: piece is not in bounds");
    }
    Piece* piece = board.getPiece(pieceData.row, pieceData.col);
    if ((pieceData.type == PieceType::Pawn && pieceData.color != ColorType::None)){
        throw std::runtime_error("Error: front and backend mismatch - expected empty square");
    }
    return true;
}