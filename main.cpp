#include <iostream>
#include <memory>

#include "board.h"
#include "dangerMap.h"
#include "pawn.h"
#include "server.h"

int main() {
    Board b;
    b.generateBoard();

    Server svr(8080);
    svr.start();
    return 0;
}