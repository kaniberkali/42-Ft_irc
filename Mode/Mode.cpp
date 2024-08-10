#include "Mode.hpp"
#include "../Exception/ClientException.hpp"
#include "../Exception/ServerException.hpp"
#include "../Message/Message.hpp"
#include "../Utils/Utils.hpp"
#include "../Command/Command.hpp"

std::string Mode::BAN = "b";
std::string Mode::INVITE_ONLY = "i";
std::string Mode::LIMIT = "l";
std::string Mode::MODERATED = "m";
std::string Mode::NO_EXT_MSG = "n";
std::string Mode::OPERATOR = "o";
std::string Mode::PRIVATE = "p";
std::string Mode::SECRET = "s";
std::string Mode::KEY = "k";
std::string Mode::TOPIC = "t";

modeInfo Mode::getMode(std::string message)
{
    return Parser::modeParse(message);
}

void Mode::execOperator(Server &server, modeInfo info)
{
    Channel *channel = server.getChannel(info.channel);
    Client *client = channel->getClient(info.parameters);
    std::vector<Client *> clients = channel->getClients();
    try
    {
        if (info.status)
            channel->addOperator(client);
        else
            channel->removeOperator(client);
        for (size_t i = 0; i < clients.size(); i++)
        {
            int targetFd = clients[i]->getFd().fd;
            if (info.status)
                Message::send(targetFd, ":" + client->getNickName() + "!" + client->getUserName() + "@" + " MODE " +
                                        channel->getName() + " +o " + info.parameters + "\r\n");
            else
                Message::send(targetFd, ":" + client->getNickName() + "!" + client->getUserName() + "@" + " MODE " +
                                        channel->getName() + " -o " + info.parameters + "\r\n");
        }
    }
    catch (ServerException::AlreadyOperatorException &e) { }
    catch (ServerException::NotAlreadyOperatorException &e) { }
}

void Mode::execBan(Server &server, modeInfo info, int fd)
{
    try
    {
        Channel *channel = server.getChannel(info.channel);
        if (info.status && info.parameters.empty())
        {
            std::vector<Client *>bans = channel->getBans();
            for (size_t i = 0; i < bans.size(); i++) 
                Message::send(fd, ":" + server.getName() + " 367 " + server.getClient(fd)->getNickName() + " " + channel->getName() + " " + bans[i]->getHostName() + "\r\n");
            Message::send(fd, ":" + server.getName() + " 368 " + server.getClient(fd)->getNickName() + " " + channel->getName() + " :End of Channel Ban List\r\n");
        }
        else
        {
            if (info.parameters.find("@") == std::string::npos)
                info.parameters = "*!*@" + info.parameters;
            if (info.status)
            {
                std::vector < Client * > ipClients = channel->getClients(Utils::split(info.parameters, "@")[1]);
                for (size_t i = 0; i < ipClients.size(); i++) 
                {
                    channel->addBan(ipClients[i]);
                    Message::send(ipClients[i]->getFd().fd,
                                ":" + server.getName() + " KICK " + channel->getName() + " " +
                                ipClients[i]->getNickName() + " :You are banned\r\n");
                }
            }
            std::vector<Client *> clients = channel->getClients();
            for (size_t i = 0; i < clients.size(); i++) 
            {
                int targetFd = clients[i]->getFd().fd;
                if (info.status)
                    Message::send(targetFd,
                                ":" + clients[i]->getNickName() + "!" + clients[i]->getUserName() + "@" + " MODE " +
                                channel->getName() + " +b " + info.parameters + "\r\n");
                else 
                {
                    Message::send(targetFd,
                                ":" + clients[i]->getNickName() + "!" + clients[i]->getUserName() + "@" + " MODE " +
                                channel->getName() + " -b " + info.parameters + "\r\n");
                }
                Command::Execute(server, "WHO " + channel->getName(), targetFd);
            }
            if (!info.status)
                channel->removeBan(channel->getBan(Utils::trim(Utils::split(info.parameters, "@")[1])));
        }
    }
    catch (ServerException::NotAlreadyBanException &e) { }
    catch (ServerException::AlreadyBanException &e) { }
}

void Mode::execLimit(Server &server, modeInfo info)
{
    Channel *channel = server.getChannel(info.channel);
    std::vector<Client *> clients = channel->getClients();
    for (size_t i = 0; i < clients.size(); i++)
    {
        int targetFd = clients[i]->getFd().fd;
        if (info.status)
        {
            Message::send(targetFd, ":" + clients[i]->getNickName() + "!" + clients[i]->getUserName() + "@" + " MODE " +
                                    channel->getName() + " +l " + info.parameters + "\r\n");
            channel->setLimit(Utils::toInt(info.parameters));
        }
        else
        {
            Message::send(targetFd, ":" + clients[i]->getNickName() + "!" + clients[i]->getUserName() + "@" + " MODE " +
                                    channel->getName() + " -l " + Utils::toString(channel->getLimit()) + "\r\n");
            channel->setLimit(NON_LIMITED);
        }
    }
}

void Mode::execInviteOnly(Server &server, modeInfo info)
{
    Channel *channel = server.getChannel(info.channel);
    std::vector<Client *> clients = channel->getClients();
    channel->setInviteOnly(info.status);
    for (size_t i = 0; i < clients.size(); i++)
    {
        int targetFd = clients[i]->getFd().fd;
        if (info.status)
            Message::send(targetFd, ":" + clients[i]->getNickName() + "!" + clients[i]->getUserName() + "@" + " MODE " +
                                    channel->getName() + " +i " + info.parameters + "\r\n");
        else
            Message::send(targetFd, ":" + clients[i]->getNickName() + "!" + clients[i]->getUserName() + "@" + " MODE " +
                                    channel->getName() + " -i " + info.parameters + "\r\n");
    }
}

void Mode::execKey(Server &server, modeInfo info)
{
    Channel *channel = server.getChannel(info.channel);
    std::vector<Client *> clients = channel->getClients();
    channel->setPassword(info.parameters);
    for (size_t i = 0; i < clients.size(); i++)
    {
        int targetFd = clients[i]->getFd().fd;
        if (info.status)
            Message::send(targetFd, ":" + clients[i]->getNickName() + "!" + clients[i]->getUserName() + "@" + " MODE " +
                                    channel->getName() + " +k " + info.parameters + "\r\n");
        else
            Message::send(targetFd, ":" + clients[i]->getNickName() + "!" + clients[i]->getUserName() + "@" + " MODE " +
                                    channel->getName() + " -k " + info.parameters + "\r\n");
    }
}

void Mode::execTopic(Server &server, modeInfo info)
{
    Channel *channel = server.getChannel(info.channel);
    std::vector<Client *> clients = channel->getClients();
    channel->setTopic(info.parameters);
    for (size_t i = 0; i < clients.size(); i++)
    {
        int targetFd = clients[i]->getFd().fd;
        if (info.status)
            Message::send(targetFd, ":" + server.getName() + " TOPIC " + channel->getName() + " :" + info.parameters + "\r\n");
        else
            Message::send(targetFd, ":" + server.getName() + " TOPIC " + channel->getName() + " :No topic is set\r\n");
    }
}

void Mode::Execute(Server &server, std::string message, int fd)
{
    if (message.find(" ") == std::string::npos)
        return;
    if (message.find(" ") != std::string::npos && Utils::split(message, " ").size() < 3)
        return;
    Client *client = server.getClient(fd);
    modeInfo info = Mode::getMode(message);
    Channel *channel = server.getChannel(info.channel);
    if (!channel->getOperator(fd))
        throw ClientException::NotOperatorException(server.getName(), fd , client->getNickName() ,channel->getName());
    info.key = Utils::toLower(info.key);
    if (info.key == OPERATOR)
        execOperator(server, info);
    else if (info.key == BAN)
        execBan(server, info, fd);
    else if (info.key == LIMIT)
        execLimit(server, info);
    else if (info.key == INVITE_ONLY)
        execInviteOnly(server, info);
    else if (info.key == KEY)
        execKey(server, info);
    else if (info.key == TOPIC)
        execTopic(server, info);
}