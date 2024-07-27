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

reciveMessage Parser::privateMessage(std::string message)
{
    reciveMessage info;
    std::vector<std::string> words = Utils::split(message, ' ');
    info.target = words[1];

    if (words[2][0] == ':')
        words[2].erase(0, 1);
    for (size_t i = 2; i < words.size(); i++)
    {
        info.message += Utils::trim(words[i]);
        if (i != words.size() - 1)
            info.message += " ";
    }
    return info;
}

parseInfo Parser::parse(std::string message)
{
    parseInfo info;

    if (message.find(" ") == std::string::npos)
    {
        info.command = message;
        return info;
    }
    std::vector<std::string> words = Utils::split(message, ' ');
    info.command = Utils::trim(words[0]);
    if (words.size() == 1)
        return info;
    info.function = Utils::trim(words[1]);
    if (words.size() == 2)
        return info;
    for (size_t i = 2; i < words.size(); i++)
    {
        info.value += Utils::trim(words[i]);
        if (i != words.size() - 1)
            info.value += " ";
    }
    return info;
}