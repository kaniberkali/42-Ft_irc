#include "Parser.hpp"
#include "../Logger/Logger.hpp"
#include <sstream>
#include "../Utils/Utils.hpp"


clientInfo Parser::connectionMessage(std::string message)
{
    clientInfo info;
    std::stringstream stream(message);
    std::string word;

    while (stream >> word)
    {
        if (word == "USER")
        {
            stream >> info.userName;
            std::getline(stream, info.realName, ':');
            std::getline(stream, info.realName);
        }
        else if (word == "NICK")
            stream >> info.nickName;
        else if (word == "PASS")
            stream >> info.password;
    }
    if (!info.realName.empty() && info.realName[0] == ' ')
        info.realName.erase(0, 1);
    info.realName = Utils::trim(info.realName);
    info.userName = Utils::trim(info.userName);
    info.nickName = Utils::trim(info.nickName);
    info.password = Utils::trim(info.password);
    return info;
}