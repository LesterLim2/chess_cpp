#include <iostream>

#include "game.h"
#include "king.h"

Game::Game(Board& b,DangerMap& d): board(b),dangerMap(d){
    playerWhite = std::make_unique<Player>(ColorType::White);
    playerBlack = std::make_unique<Player>(ColorType::Black);
    updateDangerMap();
};

std::string Game::preMove(int row, int col, ColorType color, PieceType type){
    Piece* piece = board.getPiece(row,col);
    if (piece == nullptr && color != ColorType::None && type !=PieceType::None) throw "Error: Piece mismatch";
    std::string availablePreMoves = "None";

    if(piece != nullptr){
        if(!isValidatedPiece(piece,row,col,color,type)){
            throw "Error: piece mismatch";
        }
        if(color == ColorType::None && type == PieceType::None){
            throw "test";
        }
        if (type == PieceType::King){
            availablePreMoves = static_cast<King*>(piece)->checkMovement(board, dangerMap);
        } 
        else {
        availablePreMoves = piece->checkMovement(board);
        }
    };
    return availablePreMoves; 
}

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

    board.movePiece(preRow, preCol, postRow, postCol);
    updateDangerMap();

    //
    std::pair<int,int>& kingCoordinates = movingColor == ColorType::White ? whiteKingCoordinates : blackKingCoordinates;
    if(movingPiece == PieceType::King){
        kingCoordinates = {postRow,postCol}; 
    }
    std::pair<int,int>& attackedKingCoordinates = movingColor == ColorType::White ? whiteKingCoordinates : blackKingCoordinates;
    int checkedCount = dangerMap.getDangerCount(attackedKingCoordinates.second,attackedKingCoordinates.first,movingColor);

    switch(checkedCount){
        case(0):
            break;
        case(1):
            handleCheck();
            break;
        default:
            handleMultipleCheck(attackedKingCoordinates,movingColor);
            break
    }
    if (dangerMap.getDangerCount( attackedKingCoordinates.second,attackedKingCoordinates.first,movingColor) == 1){
        std::string checkString = handleCheck(movingColor);
    };
    incrementTurn();
    return "";
}

std::string Game::handleMultipleCheck(std::pair<int,int>& attackedKingCoordinates,ColorType movingColor){
    Piece* king = board.getPiece(attackedKingCoordinates.second,attackedKingCoordinates.first);
    if(king->getType() != PieceType::King){
        throw std::runtime_error("ERROR coordinates does not contain KIng");
    }
    std::string kingMovementString = king->checkMovement(board);
    int orignalRow = attackedKingCoordinates.second;
    int originalCol = attackedKingCoordinates.first;
    bool isCapturing = true;
    std::string kingAvailableMoves = "";
    std::string kingAvailableCaptures = "c";

    if(kingMovementString[-1] != 'c'){
        std::vector<std::vector<int>> originalDangerMap = dangerMap.getDangerMap(movingColor);
    }
    for(int i = 0; i < kingMovementString.size(); i++){
        if(kingMovementString[i] == 'c'){
            isCapturing = true;
            continue;
        }
        if(kingMovementString[i] == '&'){
            int moveRow = static_cast<int>(kingMovementString[i-2]);
            int moveCol = static_cast<int>(kingMovementString[i-1]);
            if(!isCapturing){
                if (dangerMap.getDangerCount(moveRow,moveCol,movingColor) == 0){
                    kingAvailableMoves += kingMovementString.substr(i-2,3);
                }
            }
            if(isCapturing){
                Piece* piece = board.getPiece(moveRow,moveCol);
                if(piece == nullptr){
                    throw("error capturing a piece that does exist @game::handlemultiplecheck");
                }
                //simulateMovement
                board.movePiece(orignalRow,originalCol,moveRow,moveCol);
                updateDangerMap();

                if(dangerMap.getDangerCount(moveRow,moveCol,movingColor) == 0){
                    kingAvailableCaptures += kingMovementString.substr(i-2,3);
                }
                //move King back to original position.
                board.movePiece(moveRow,moveCol,orignalRow,originalCol);
                //restore captured piece.
                board.placePiece()...
                dangerMap.setDangerMap(originalDangerMap,movingColor);
            }
        }
    }
}
std::string Game::handleCheck(ColorType checkedColor){
    std::string uncheckPiecesString = "";
    for(int row = 0 ; row < 8;row++){
        for(int  col = 0;col < 8;col++){
            Piece* piece = board.getPiece(row,col);
            if(piece == nullptr) continue;
            if(piece->getColor() != checkedColor) continue;

            std::vector<std::vector<int>> simulatedDangerMap = dangerMap.getDangerMap(checkedColor);
            std::vector<std::vector<std::unique_ptr<Piece>>> simulatedBoard = board.getBoard();
            auto [originalRow,originalCol] = piece->getPosition();

            simulateUncheckMoves(piece,simulatedDangerMap,simulatedBoard);
        }
    }

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
    updateDangerMap();
    return "";
}

std::string Game::castling(const std::string& castleString){

    PieceData kingData = parsePieceString(castleString.substr(0,4));
    PieceData rookData = parsePieceString(castleString.substr(4,4));
    try{
        validatePieceMovement(kingData);
        validatePieceMovement(rookData);
    }
    catch(const std::exception& e){
        std::cout << "an exception has occured: " << e.what() << std::endl;
    }

    int kingDestCol =  rookData.col == 0 ? 2 : 6;
    int rookDestCol = rookData.col == 0 ? 3 : 5;

    //king movement
    board.movePiece(kingData.row,kingData.col,kingData.row,kingDestCol);
    //rook movement
    board.movePiece(rookData.row,rookData.col,rookData.row,rookDestCol);
    
    updateDangerMap();
    incrementTurn();
    return "validated";
    //toDO dangerMapupdate
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

void Game::updateDangerMap(){
    std::vector<std::vector<int>> dummyDMapWhite = dangerMap.createDummyDangerMap();
    std::vector<std::vector<int>> dummyDMapBlack = dangerMap.createDummyDangerMap(); 
    for(int row = 0; row < 8; row++){
        for(int col = 0; col < 8; col++){
            Piece* piece = board.getPiece(row,col);
            if (piece == nullptr){
                continue;
            }
            std::vector<std::vector<int>>& affectedDangerMap = piece->getColor() == ColorType::White ? dummyDMapWhite : dummyDMapBlack;
            std::vector<std::pair<int,int>> dangerTiles = piece->getDanger(board);

            for(auto& danger : dangerTiles){
                affectedDangerMap[danger.first][danger.second]++;
            }
        }
    }
    dangerMap.setDangerMap(dummyDMapWhite,ColorType::White);
    dangerMap.setDangerMap(dummyDMapBlack,ColorType::Black);
    
    std::cout<< "dangerMaps post movement" << std::endl;
    std::cout << "white\n" <<std::endl;
    dangerMap.stateDangerMap(ColorType::White);
    std::cout << "black\n" <<std::endl;
    dangerMap.stateDangerMap(ColorType::Black);
    return;
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