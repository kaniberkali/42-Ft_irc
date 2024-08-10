#ifndef MODE_HPP
# define MODE_HPP

#include "../Parser/Parser.hpp"
#include "../Server/Server.hpp"

class Mode
{
    public:
        static std::string BAN;
        static std::string INVITE_ONLY;
        static std::string LIMIT;
        static std::string MODERATED;
        static std::string NO_EXT_MSG;
        static std::string OPERATOR;
        static std::string PRIVATE;
        static std::string SECRET;
        static std::string KEY;
        static std::string TOPIC;

        static modeInfo getMode(std::string message);
        static void execOperator(Server &server, modeInfo info);
        static void execBan(Server &server, modeInfo info, int fd);
        static void execLimit(Server &server, modeInfo info);
        static void execInviteOnly(Server &server, modeInfo info);
        static void execKey(Server &server, modeInfo info);
        static void execTopic(Server &server, modeInfo info);
        static void Execute(Server &server, std::string message, int fd);
};

#endif