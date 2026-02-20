#pragma once 

#include <iostream>
#include <httplib.h>

#include "piece.h"

class Board;
class Server{
    private:
        httplib::Server svr;
        int port;
    public:
        Server(int port);
        void setUpRoutes();
        void start();

};
