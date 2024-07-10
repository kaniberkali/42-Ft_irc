
#include <iostream>
#include "Check/Check.hpp"
#include "Server/Server.hpp"

int main(int ac, char **av)
{
    if (ac == 3)
    {
        int portNum = atoi(av[1]);
        std::string pass = av[2];
        
        try {
            Check checker(portNum, pass);
        } catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
        
        Server Server(portNum);

    }
    else {
        std::cerr << "yetersiz argüman sayisi" << std::endl;
    }


    return (0);
}