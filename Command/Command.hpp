#ifndef COMMAND_HPP
# define COMMAND_HPP

#include <iostream>
#include "../Server/Server.hpp"

class Command
{
    public:
        static std::string QUIT;
        static std::string JOIN;
        static std::string CAP_LS;
        static std::string PRIVMSG;
        static std::string PART;
        static std::string WHO;
        static std::string getCommand(std::string message);
        static void Execute(Server &server, std::string message, int fd);
        static void execCapLs(Server &server, std::string message, int fd);
        static void execQuit(Server &server, int fd);
        static void execPart(Server &server, std::string message , int fd);

        static void execPrivMsg(Server &server, std::string message, int fd);
        static void sendMessage(const std::string& target, const std::string& message, int senderFd);
        static void execJoin(Server &server,std::string message, int fd);
        static void execWho(Server &server, std::string message, int fd);
};

#endif