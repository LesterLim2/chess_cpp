#include <iostream>
#include <memory>

#include "board.h"
#include "dangerMap.h"
#include "pawn.h"
#include "server.h"
#include "game.h"

int main() {
    Board b;
    b.generateBoard();

    Game game(b);

    Server server(8080,game);
    server.start();
}