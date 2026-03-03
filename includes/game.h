#pragma once

#include <iostream>
#include "types.h"


class Board;
class DangerMap;
class Piece;

//handles game types(singleplayer (probably not going to be implemented))
//handles higher level game logic type shit, does this need to exist?
class Game{
    private:
        Board& board;
        DangerMap& dangerMap;
        std::pair<int,int> whiteKingCoordinates;
        std::pair<int,int> blackKingCoordinates;
        int turn = 1;
    public:
        Game(Board& board,DangerMap& dangerMap);
        std::string preMove(int row, int col, ColorType color, PieceType type);
        std::string getMovementString(int row,int col);
        bool isValidatedPiece(Piece* piece, int row, int col, ColorType color, PieceType type);
        bool validatePieceMovement(const std::string& pieceStr);
        
        std::string movePiece(std::string pieceString);
        bool checkPromotion(ColorType movingColor,int PostROw);
        std::string getPossibleunCheckMoves(ColorType opponentColor, std::pair<int,int> kingCoordinates, std::pair<int,int> checkerCoordinates, PieceType checkerType);
        std::string simulateUncheckMoves(Piece* currentPiece, std::pair<int,int> kingCoordinates, std::pair<int,int> checkerCoordinates, PieceType checkerType);
};