#include <iostream>
#include <memory>

#include "board.h"
#include "dangerMap.h"
#include "pawn.h"
#include "server.h"

int main() {
    svr.Post("/button-clicked", [](const httplib::Request& req, httplib::Response& res) {
        std::cout << "Button was clicked!" << std::endl;
        std::cout << "Request body: " << req.body << std::endl;
        
        // Send response back to Flask
        res.set_content("Message received by C++!", "text/plain");
    });


    std::cout << "C++ server listening on http://localhost:8080" << std::endl;  
    svr.listen("0.0.0.0", 8080);  // Listen on port 8080
    return 0;
}