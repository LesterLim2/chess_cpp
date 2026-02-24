#pragma once

#include <iostream>
#include "types.h"


class Board;
class Piece;

//handles game types(singleplayer (probably not going to be implemented))
//handles higher level game logic type shit, does this need to exist?
class Game{
    private:
        Board& board;
    public:
        Game(Board& board);
        void checkAvailableMoves(Board& board);
        std::string preMove(int row, int col, ColorType color, PieceType type);
        bool isValidatedPiece(Piece* piece, int row, int col, ColorType color, PieceType type);
        std::string vectorToString(std::vector<std::pair<int,int>> vector);
        
        std::string movePiece(std::string pieceString);
        std::string pieceStringToPieces(std::string pieceString);
};