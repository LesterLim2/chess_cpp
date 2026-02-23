#pragma once 

#include <iostream>
#include <httplib.h>

#include "piece.h"



class Game;
class Server{
    private:
        httplib::Server svr;
        int port;
        Game& game;
    public:
        Server(int port,Game& game);
        void setUpRoutes();
        void start();

};
