#include "Check.hpp"
#include "../Server/Server.hpp"
#include <stdlib.h>

Check::Check() {
    std::cout << "Check constructor" << std::endl;
}

Check::~Check() {
    std::cout << "Check destructor" << std::endl;
}

int Check::CheckPort(std::string port) { 

    for (size_t i = 0; i < port.length(); i++)
    {
        if (!isdigit(port[i]))
            throw std::invalid_argument("Port number must be a number");
    }

    int portNum = atoi(port.c_str());

    if (portNum < MIN_PORT || portNum > MAX_PORT)
        throw std::invalid_argument("Port number must be between 1 and 65535");

    return portNum;
}


