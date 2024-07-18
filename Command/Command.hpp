#ifndef COMMAND_HPP
# define COMMAND_HPP

#include <iostream>
#include "../Server/Server.hpp"

class Command
{
    public:
        static std::string QUIT;
        static std::string USER;
        static std::string NICK;
        static std::string PASS;
        static std::string JOIN;
        static std::string CAP_LS;
        static std::string getCommand(std::string message);
        static void Perform(Server &server, std::string message, int fd);
        static void execCapLs(Server &server, std::string message, int fd);
        static void execQuit(Server &server, int fd);
};

#endif