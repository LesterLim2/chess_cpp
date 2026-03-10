#pragma once

#include <iostream>
#include "types.h"
#include "board.h"
#include "dangerMap.h"
#include "piece.h"
#include "player.h"
class Piece;
class Player;

//handles game types(singleplayer (probably not going to be implemented))
//handles higher level game logic type shit, does this need to exist?
class Game{
    private:
        Board& board;
        DangerMap& dangerMap;
        std::pair<int,int> whiteKingCoordinates;
        std::pair<int,int> blackKingCoordinates;
        std::unique_ptr<Player> playerWhite;
        std::unique_ptr<Player> playerBlack;
        int turn = 1;
        ColorType currentTurn;
    public:
        Game(Board& board,DangerMap& dangerMap);
        std::string preMove(int row, int col, ColorType color, PieceType type);
        std::string getMovementString(int row,int col);
        void incrementTurn();
        bool isValidatedPiece(Piece* piece, int row, int col, ColorType color, PieceType type);
        bool validatePieceMovement(const std::string& pieceStr);
        
        std::string movePiece(std::string pieceString);
        bool checkPromotion(ColorType movingColor,int PostROw);
        std::string getPossibleunCheckMoves(ColorType opponentColor, std::pair<int,int> kingCoordinates, std::pair<int,int> checkerCoordinates, PieceType checkerType);
        std::string simulateUncheckMoves(Piece* currentPiece, std::pair<int,int> kingCoordinates, std::pair<int,int> checkerCoordinates, PieceType checkerType);

        void updateBlockPieces(int preRow,int preCol,ColorType movingColor,int postRow,int postCol);
        std::string handleCheck(std::pair<int,int>,PieceType checkerType);
        std::string handleEnPessant();
};