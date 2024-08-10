#include "Parser.hpp"
#include "../Logger/Logger.hpp"
#include <sstream>
#include "../Utils/Utils.hpp"

reciveMessage Parser::privateMessage(std::string message)
{
    reciveMessage info;
    std::vector<std::string> words = Utils::split(message, " ");
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
    std::vector<std::string> words = Utils::split(message, " ");
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

userInfo Parser::userParse(std::string message)
{
    userInfo user;
    std::vector<std::string> words = Utils::split(message, " ");
    user.userName = words[1];
    user.realName = words[4].replace(0, 1, "");
    return user;
}

modeInfo Parser::modeParse(std::string message)
{
    modeInfo info;
    std::vector<std::string> words = Utils::split(message, " ");
    info.channel = Utils::trim(words[1]);
    info.status = words[2][0] == '+';
    info.key = words[2][1];
    if (words.size() == 3)
        return info;
    info.parameters = Utils::trim(words[3]);
    if (words.size() == 4)
        return info;
    if (words.size() == 5)
        info.parameters += Utils::trim(words[4]);
    return info;
}

