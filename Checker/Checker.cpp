#include "Checkerer.hpp"
#include "../Server/Server.hpp"
#include <stdlib.h>
#include "../Exception/ExceptionMsg.hpp"

Checker::Checker() {
    std::cout << "Checker constructor" << std::endl;
}

Checker::~Checker() {
    std::cout << "Checker destructor" << std::endl;
}

int Checker::CheckerPort(std::string port) {

    for (size_t i = 0; i < port.length(); i++)
    {
        if (!isdigit(port[i]))
            throw std::invalid_argument("Port number must be a number");
    }


    int portNum = atoi(port.c_str());

    if (portNum < MIN_PORT || portNum > MAX_PORT)
        throw Exception::Server(INVALID_PORT);

    return portNum;
}


