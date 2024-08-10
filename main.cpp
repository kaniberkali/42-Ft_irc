
#include <iostream>
#include "Checker/Checker.hpp"
#include "Server/Server.hpp"
#include "Logger/Logger.hpp"
#include "Exception/ServerException.hpp"
#include "Exception/ClientException.hpp"

int main(int ac, char **av)
{
    if (ac == 3) 
    {
        std::string port = av[1];
        std::string pass = av[2];

        try 
        {
            int portNum = Checker::port(port);
            Server server(portNum, pass);
        } 
        catch (const ServerException &e) 
        {
            std::cerr << e.what() << std::endl;
        }
        catch (const ClientException &e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
    else
        Logger::Error("number of arguments missing or excessive");
    return (0);
}
