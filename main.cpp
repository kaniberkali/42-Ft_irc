
#include <iostream>
#include "Check/Check.hpp"
#include "Server/Server.hpp"

int main(int ac, char **av)
{   
    if (ac == 3)
    {
        std::string port = av[1];
        std::string pass = av[2];

        try {
            int portNum = Check::CheckPort(port);
            Server server(portNum, pass);
        } catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
    }
    else {
        std::cerr << "yetersiz argüman sayisi" << std::endl;
    }

    std::cout << "I am here " << std::endl; // server burada sonlandırılır 

    return (0);
}