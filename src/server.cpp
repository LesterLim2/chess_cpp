#include <iostream>
#include <string>

#include "server.h"

Server::Server(int p) : port(p) {
    setUpRoutes();
    std::cout << "Server finished construction" << std::endl;
};

void Server::setUpRoutes(){
    svr.Get("/tile-clicked", [](const httplib::Request& req, httplib::Response& res) {
        int row = std::stoi(req.get_param_value("row"));
        int col = std::stoi(req.get_param_value("col"));
        PieceType type = stringToPieceType[req.get_param_value("pieceType")];
        ColorType color = stringToColorType[req.get_param_value("colorType")];

        std::cout << "Tile clicked: row=" << row << " col=" << col 
          << " type=" << req.get_param_value("pieceType")
          << " color=" << req.get_param_value("colorType") << std::endl;

        res.set_content("Tile received: row=" + std::to_string(row) + " col=" + std::to_string(col) + " type=" + req.get_param_value("pieceType") + " color=" + req.get_param_value("colorType"), "text/plain");

        
    });
}

void Server::start(){
    std::cout << "Server listening on http://localhost:" << port << std::endl;
    bool success = svr.listen("0.0.0.0", port);
    if(!success){
        std::cout << "Failed to start server on port " << port << std::endl;
    }
}
