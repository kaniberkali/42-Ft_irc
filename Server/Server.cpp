#include "Server.hpp"

Server::Server() {
    std::cout << "Server constructor" << std::endl;
}

Server::~Server() {
    std::cout << "Server destructor" << std::endl;
}

Server::Server(int port, std::string pass) : _port(port), _pass(pass) {
    std::cout << "Server constructor" << std::endl;

    std::cout << "Port: " << _port << std::endl;
    std::cout << "Pass: " << _pass << std::endl;

    
}