
#include <iostream>
#include "Checker/Checker.hpp"
#include "Server/Server.hpp"
#include "Exception/ExceptionMsg.hpp"


int main(int ac, char **av) {
    if (ac == 3) {
        std::string port = av[1];
        std::string pass = av[2];

        try {
            int portNum = Checker::CheckerPort(port);
            Server server(portNum, pass);
        } catch (Exception::Server &e) {
            std::cerr << e.what() << std::endl;
        } catch (Exception::Client &e) {
            std::cerr << e.what() << std::endl;
        }
    }
    else
        std::cout << "arguman sayisi eksik" << std::endl;

    // server sonlandırma

    return (0);
}
