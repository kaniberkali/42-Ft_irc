#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <sstream>

struct userInfo {
    std::string nickName;
    std::string password;
    std::string userName;
    std::string realName;
    std::string hostName;
};

struct serverInfo
{
    std::string name;
    std::string password;
    std::string version;
    std::string createDate;
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

struct modeInfo
{
    std::string channel;
    bool status;
    std::string key;
    std::string parameters;
};

class Parser
{
    public:
        static reciveMessage privateMessage(std::string message);
        static parseInfo parse(std::string  message);
        static userInfo userParse(std::string message);
        static modeInfo modeParse(std::string message);
};

#endif
