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

        std::string preMoveString = "None";
        try{
            preMoveString = game.preMove(row,col,color,type);
        } catch(const std::exception& e){
            std::cout << "preMove exception: " << e.what() << std::endl;
        } catch(...){
            std::cout << "preMove unknown exception" << std::endl;
        }
        res.set_content(preMoveString,"text/plain");
    });

    svr.Get("/move-piece", [&](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin","*");
        std::string pieceString = req.get_param_value("pieces");
        std::string moveResultString = "";
        try{
            moveResultString = game.movePiece(pieceString);
        }
        catch(const std::exception &e){
            std::cout << "an exception has occured :" << e.what() << std::endl;
        }
        res.set_content(moveResultString,"text/plain");

        std::cout << "Piece moved: row=" << (pieceString[4]-'0') << " col=" << (pieceString[5]-'0')
          << " type=" << pieceString[2]
          << " color=" << pieceString[3] << std::endl;
    });

    svr.Get("/pawn-promotion", [&](const httplib::Request& req, httplib::Response& res){
        res.set_header("Access-Control-Allow-Origin","*");

        std::string promoteString = req.get_param_value("promoteValues");
        if (promoteString.size() != 4){
            std::cout << "Error promoteString size does not match requirements" <<std::endl;
            res.set_content("Error","text/plain");
        }
        res.set_content(game.pawnPromotion(promoteString),"text/plain");
        });
    
    svr.Get("/castling",[&](const httplib::Request& req,httplib::Response& res){
        res.set_header("Access-Control-Allow-Origin","*");

        std::string castleString = req.get_param_value("pieces");
        std::cout << "castling content recieved, castleString = " << castleString << std::endl;
        res.set_content(game.castling(castleString),"text/plain");
    });
}
void Server::start(){
    std::cout << "Server listening on http://localhost:" << port << std::endl;
    bool success = svr.listen("0.0.0.0", port);
    if(!success){
        std::cout << "Failed to start server on port " << port << std::endl;
    }
}
