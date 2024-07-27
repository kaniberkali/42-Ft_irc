#include "Checker.hpp"
#include "../Server/Server.hpp"
#include <stdlib.h>
#include "../Exception/ServerException.hpp"
#include "../Exception/ClientException.hpp"

int Checker::port(std::string port)
{
    for (size_t i = 0; i < port.length(); i++)
    {
        if (!isdigit(port[i]))
            throw ServerException::PortDigitException();
    }

    int portNum = atoi(port.c_str());
    if (portNum < MIN_PORT || portNum > MAX_PORT)
        throw ServerException::PortRangeException();
    return portNum;
}


