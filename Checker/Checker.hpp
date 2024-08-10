#ifndef Checker_HPP
# define Checker_HPP

#include <iostream>
#include "../Server/Server.hpp"

class Checker
{   
    public:
        static std::string JOIN;
        static std::string PART;
        static std::string WHOIS;
        static std::string WHO;
        static std::string NICK;
        static std::string INVITE;
        static std::string QUIT;
        static std::string PRIVMSG;
        static std::string MODE;
        static std::string KICK;
        static std::string TOPIC;
        static std::string NOTICE;
        static std::string PASS;
        static std::string USER;
        static int port(std::string port);
        static bool checkOnlyChannel(std::string message);
        static bool checkOnlyMode(std::string format);
        static bool checkOnlyNickName(std::string message);
        static bool checkOnlyNickAndChannelName(std::string message);
        static bool checkOnlyChannelAndMessage(std::string message);
        static bool checkOnlyNickAndMode(std::string message);
        static bool checkMoreThanTwoWords(std::string message);
        static bool checkOnlyChannelOrNickName(std::string message);
        static bool checkFirstParamIsChannel(std::string message);
        static std::string getCommand(std::string message);
        static bool check(Server &server, std::string message, int fd);
};

#endif
