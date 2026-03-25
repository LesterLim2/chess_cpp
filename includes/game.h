#pragma once

#include <iostream>
#include "types.h"
#include "board.h"
#include "dangerMap.h"
#include "piece.h"
#include "player.h"
class Piece;
class Player;

struct PieceData {
    int row;
    int col;
    PieceType type;
    ColorType color;
};

//handles game types(singleplayer (probably not going to be implemented))
//handles higher level game logic type shit, does this need to exist?
class Game{
    private:
        Board& board;
        DangerMap& dangerMap;
        std::pair<int,int> whiteKingCoordinates = {0, 4};
        std::pair<int,int> blackKingCoordinates = {7, 4};
        std::unique_ptr<Player> playerWhite;
        std::unique_ptr<Player> playerBlack;
        int turn = 1;
        ColorType currentTurn;
        Piece* enPassantTargetWhite = nullptr;
        Piece* enPassantTargetBlack = nullptr;
        int enPassantCounterWhite = 0;
        int enPassantCounterBlack = 0;
    public:
        Game(Board& board,DangerMap& dangerMap);
        std::string preMove(int row, int col, ColorType color, PieceType type);
        std::string getMovementString(int row,int col);
        void incrementTurn();
        bool isValidatedPiece(Piece* piece, int row, int col, ColorType color, PieceType type);
        bool validatePieceMovement(const PieceData& pieceData);
        
        PieceData parsePieceString(const std::string& s);
        std::string movePiece(std::string pieceString);
        std::string getPossibleunCheckMoves(ColorType opponentColor, std::pair<int,int> kingCoordinates, std::pair<int,int> checkerCoordinates, PieceType checkerType);
        std::string simulateUncheckMoves(Piece* currentPiece, std::pair<int,int> kingCoordinates, std::pair<int,int> checkerCoordinates, PieceType checkerType);

        void updateBlockPieces(int preRow,int preCol,ColorType movingColor,int postRow,int postCol);
        std::string handleCheck(ColorType checkedColor);
        std::string handleMultipleCheck(std::pair<int,int>& attackedKingCoordinates,ColorType movingColor);
        std::string checkEnPassant(int row, int col, ColorType color);

        std::string pawnPromotion(const std::string& promotionPiece);
        bool validatePawnPromotion(const PieceData& pieceData);

        std::string castling(const std::string& castleString);
        
        std::string simulateUncheckMoves(Piece* piece, std::vector<std::vector<int>> simulatedDangerMap,std::vector<std::vector<std::unique_ptr<Piece>>> simulatedBoard);
        void updateDangerMap();
};