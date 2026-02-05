#include "board.h"
#include "pawn.h"
#include "piece.h"
#include <memory>
#include <iostream>

int main() {
    Board board;

    // White pawn at (3,1)
    auto whitePawn = std::make_unique<Pawn>(
        ColorType::White,
        std::pair<int,int>{3, 1}
    );

    // Black pieces placed diagonally in front
    auto blackPawnRight = std::make_unique<Pawn>(
        ColorType::Black,
        std::pair<int,int>{4, 2}
    );

    auto blackPawnLeft = std::make_unique<Pawn>(
        ColorType::Black,
        std::pair<int,int>{2, 2}
    );

    // Friendly piece directly in front (should NOT be capturable)
    auto whiteBlocker = std::make_unique<Pawn>(
        ColorType::White,
        std::pair<int,int>{3, 2}
    );

    // Place everything on the board
    board.placePiece(std::move(whitePawn));
    board.placePiece(std::move(blackPawnRight));
    board.placePiece(std::move(blackPawnLeft));
    board.placePiece(std::move(whiteBlocker));

    // Get the pawn back from the board
    Pawn* pawn = dynamic_cast<Pawn*>(board.getPiece(3, 1));
    if (!pawn) {
        std::cout << "ERROR: Pawn not found on board\n";
        return 1;
    }

    std::cout << "Checking capture moves for pawn at (3,1)\n";
    pawn->checkCapture(board);

    board.stateBoard();

    return 0;
}