#ifndef COMMAND_HPP
# define COMMAND_HPP

#include <iostream>
#include "../Server/Server.hpp"


class Command
{
    public:
        static std::string QUIT;
        static std::string JOIN;
        static std::string PRIVMSG;
        static std::string PART;
        static std::string WHO;
        static std::string NICK;
        static std::string USER;
        static std::string PASS;
        static std::string NOTICE;
        static std::string MODE;
        static std::string KICK;
        static std::string INVITE;
        static std::string TOPIC;
        static std::string WHOIS;

        static std::string getCommand(std::string message);
        static void Execute(Server &server, std::string message, int fd);
        static void execQuit(Server &server, std::string message, int fd);
        static void execPart(Server &server, std::string message , int fd);
        static void execPrivMsg(Server &server, std::string message, int fd);
        static void sendMessage(const std::string& target, const std::string& message, int senderFd);
        static void execJoin(Server &server,std::string message, int fd);
        static void execWho(Server &server, std::string message, int fd);
        static void execNick(Server &server, std::string message, int fd);
        static void execUser(Server &server, std::string message, int fd);
        static void execPass(Server &server, std::string message, int fd);
        static void execNotice(Server &server, std::string message, int fd);
        static void execMode(Server &server, std::string message, int fd);
        static void execKick(Server &server, std::string message, int fd);
        static void execInvite(Server &server, std::string message, int fd);
        static void execTopic(Server &server, std::string message, int fd);
        static void execWhois(Server &server, std::string message, int fd);

};

#endif