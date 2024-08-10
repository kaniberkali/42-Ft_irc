#include "Checker.hpp"
#include "../Server/Server.hpp"
#include <stdlib.h>
#include "../Exception/ServerException.hpp"
#include "../Exception/ClientException.hpp"

std::string Checker::JOIN = "JOIN";
std::string Checker::PART = "PART";
std::string Checker::WHOIS = "WHOIS";
std::string Checker::WHO = "WHO";
std::string Checker::NICK = "NICK";
std::string Checker::INVITE = "INVITE";
std::string Checker::QUIT = "QUIT";
std::string Checker::PRIVMSG = "PRIVMSG";
std::string Checker::MODE = "MODE";
std::string Checker::KICK = "KICK";
std::string Checker::TOPIC = "TOPIC";
std::string Checker::NOTICE = "NOTICE";
std::string Checker::PASS = "PASS";
std::string Checker::USER = "USER";
int Checker::port(std::string port)
{
    for (size_t i = 0; i < port.length(); i++)
    {
        if (!isdigit(port[i]))
            throw ServerException::PortDigitException();
    }

    int portNum = atoi(port.c_str());
    if (portNum < MIN_PORT || portNum > MAX_PORT)
        throw ServerException::PortRangeException();
    return portNum;
}

bool Checker::checkOnlyChannel(std::string format)
{
    if (format[0] != '#')
        return false;
    return (format.find(' ') == std::string::npos);
}

bool Checker::checkOnlyNickName(std::string format)
{
    if (format[0] == '#')
        return false;
    return (format.find(' ') == std::string::npos);
}

bool Checker::checkOnlyNickAndChannelName(std::string format)
{
    if (format[0] == '#')
        return false;
    size_t index = format.find(' ');
    if (index == std::string::npos)
        return false;
    std::string nick = format.substr(0, index);
    std::string channel = format.substr(index + 1);
    if (channel[0] != '#')
        return false;
    return (channel.find(' ') == std::string::npos && nick.find(' ') == std::string::npos);
}

bool Checker::checkOnlyChannelAndMessage(std::string format)
{
    size_t index = format.find(' ');
    if (index == std::string::npos)
        return false;
    std::string channel = format.substr(0, index);
    std::string message = format.substr(index + 1);
    if (channel[0] != '#')
        return false;
    return (channel.find(' ') == std::string::npos);
}

bool Checker::checkOnlyMode(std::string format)
{
    size_t index = format.find(' ');
    if (index == std::string::npos)
        return (format[0] == '#');

    std::string channel = format.substr(0, index);
    std::string mode = format.substr(index + 1);

    if (channel[0] != '#')
        return false;

    if (mode[0] != '+' && mode[0] != '-')
        return false;

    char modeChar = mode[1];
    if (modeChar != 'o' && modeChar != 'i' && modeChar != 'b' && modeChar != 'k' && modeChar != 't' && modeChar != 'l')
        return false;

    if (modeChar == 'l' && mode[0] == '+')
        return (mode.length() > 2 && std::atoi(mode.substr(2).c_str()) != 0);
    else if (modeChar == 'l' && mode[0] == '-')
        return (mode.length() == 2);
    return (channel.find(' ') == std::string::npos);
}

bool Checker::checkMoreThanTwoWords(std::string message)
{
    size_t index = message.find(' ');
    if (index == std::string::npos)
        return false;
    std::vector<std::string>words = Utils::split(message, " ");
    return (words.size() >= 2);
}

bool Checker::checkOnlyChannelOrNickName(std::string format)
{
    return (format.find(' ') == std::string::npos);
}

bool Checker::checkFirstParamIsChannel(std::string format)
{
    return (format[0] == '#');
}

std::string Checker::getCommand(std::string message)
{
    message = Utils::toUpper(message);
    size_t index = message.find(' ');
    if (index == std::string::npos)
        return message;
    return message.substr(0, index);
}

bool Checker::check(Server &server, std::string message, int fd)
{
    Client *client = server.getClient(fd);
    std::string command = getCommand(message);
    message = Utils::trim(message);
    if (message.find(" ") == std::string::npos)
        throw ClientException::UnknownCommandException(server.getName(), fd, message, client->getNickName());
    std::vector<std::string>words = Utils::split(message, " ");
    std::string format = "";
    for (size_t i = 1; i < words.size(); i++)
        format += (i == 1) ? words[i] : " " + words[i];
    format = Utils::trim(format);
    if (command == WHO)
    {
        if (!checkOnlyChannel(format))
            throw ServerException::OnlyChannelException();
    }
    if (command == NICK)
    {
        if (!checkOnlyNickName(format))
            throw ServerException::OnlyNickNameException();
    }
    if (command == INVITE)
    {
        if (!checkOnlyNickAndChannelName(format))
            throw ServerException::OnlyNickAndChannelNameException();
    }
    if (command == QUIT)
    {
        if (format == "")
            throw ServerException::OnlyMessageException();
    }
    if (command == TOPIC || command == KICK || command == PART)
    {
        if (!checkOnlyChannelAndMessage(format))
            throw ServerException::OnlyChannelAndMessageException();
    }
    if (command == MODE)
    {
        if (!checkOnlyMode(format))
            throw ServerException::OnlyModeException();
    }
    if (command == NOTICE || command == PRIVMSG)
    {
        if (!checkMoreThanTwoWords(message))
            throw ServerException::MoreThanTwoWordsException();
    }
    if (command == WHOIS)
    {
        if (!checkOnlyChannelOrNickName(format))
            throw ServerException::OnlyChannelOrNickNameException();
    }
    if (command == JOIN)
    {
        if (!checkFirstParamIsChannel(format))
            throw ServerException::FirstParamIsChannel();
    }
    if (command != WHO && command != PASS && command != USER && command != NICK && command != INVITE && command != QUIT && command != PRIVMSG && command != TOPIC && command != KICK && command != PART && command != MODE && command != NOTICE && command != WHOIS && command != JOIN)
        throw ClientException::UnknownCommandException(server.getName(), fd, Utils::split(message, " ")[0], client->getNickName());
    return (true);
}


