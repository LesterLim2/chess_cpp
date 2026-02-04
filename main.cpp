#include "board.h"
#include "pawn.h"
#include "types.h"

#include <iostream>
#include <utility>

using namespace std;

int main() {
    Board board;

    auto pawn = make_unique<Pawn>(
        ColorType::White,
        pair<int,int>{0,1}
    );

    board.placePiece(move(pawn));
    board.stateBoard();

}
