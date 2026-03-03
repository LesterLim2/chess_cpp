#include <iostream>
#include <string>

#include "game.h"
#include "server.h"

Server::Server(int p, Game& g) : port(p),game(g) {
    setUpRoutes();
    std::cout << "Server finished construction" << std::endl;
};

void Server::setUpRoutes(){
    svr.Get("/tile-clicked", [&](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin","*");
        int row = std::stoi(req.get_param_value("row"));
        int col = std::stoi(req.get_param_value("col"));
        PieceType type = stringToPieceType[req.get_param_value("pieceType")];
        ColorType color = stringToColorType[req.get_param_value("colorType")];

        std::cout << "Tile clicked: row=" << row << " col=" << col 
          << " type=" << req.get_param_value("pieceType")
          << " color=" << req.get_param_value("colorType") << std::endl;

        std::string preMoveString = game.preMove(row,col,color,type);
        res.set_content(preMoveString,"text/plain");
    });

    svr.Get("/move-piece", [&](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin","*");
        std::string pieceString = req.get_param_value("pieces");
        res.set_content(game.movePiece(pieceString),"text/plain");

        std::cout << "Piece moved: row=" << (pieceString[4]-'0') << " col=" << (pieceString[5]-'0')
          << " type=" << pieceString[2]
          << " color=" << pieceString[3] << std::endl;
    });
}
void Server::start(){
    std::cout << "Server listening on http://localhost:" << port << std::endl;
    bool success = svr.listen("0.0.0.0", port);
    if(!success){
        std::cout << "Failed to start server on port " << port << std::endl;
    }
}
