#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <sstream>

struct clientInfo {
    std::string userName;
    std::string password;
    std::string nickName;
    std::string realName;
};

struct reciveMessage
{
    std::string target;
    std::string message;
};

struct parseInfo
{
    std::string command;
    std::string function;
    std::string value;
};

class Parser
{
    public:
        static clientInfo connectionMessage(std::string message);
        static reciveMessage privateMessage(std::string message);
        static parseInfo parse(std::string  message);
};

#endif
