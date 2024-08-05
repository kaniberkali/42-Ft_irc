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
std::string Mode::TOPIC = "t";
std::string Mode::KEY = "k";

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

void Mode::execBan(Server &server, modeInfo info)
{
    Channel *channel = server.getChannel(info.channel);
    std::vector<Client *> clients = channel->getClients();
    try
    {
        if (info.status)
        {
            std::vector<Client *> ipClients = channel->getClients(Utils::split(info.parameters, "@")[1]);
            for (size_t i = 0; i < ipClients.size(); i++)
            {
                channel->addBan(ipClients[i]);
                Message::send(ipClients[i]->getFd().fd, ":" + server.getName() + " KICK " + channel->getName() + " " + ipClients[i]->getNickName() + " :You are banned\r\n");
            }
        }
        for (size_t i = 0; i < clients.size(); i++)
        {
            int targetFd = clients[i]->getFd().fd;
            if (info.status)
                Message::send(targetFd, ":" + clients[i]->getNickName() + "!" + clients[i]->getUserName() + "@" + " MODE " +
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
    catch (ServerException::NotAlreadyBanException &e) { }
    catch (ServerException::AlreadyBanException &e) { }
}

void Mode::execLimit(Server &server, modeInfo info)
{
    Channel *channel = server.getChannel(info.channel);
    std::vector<Client *> clients = channel->getClients();
    channel->setLimit(Utils::toInt(info.parameters));
    for (size_t i = 0; i < clients.size(); i++)
    {
        int targetFd = clients[i]->getFd().fd;
        if (info.status)
            Message::send(targetFd, ":" + clients[i]->getNickName() + "!" + clients[i]->getUserName() + "@" + " MODE " +
                                    channel->getName() + " +l " + info.parameters + "\r\n");
        else
            Message::send(targetFd, ":" + clients[i]->getNickName() + "!" + clients[i]->getUserName() + "@" + " MODE " +
                                    channel->getName() + " -l " + info.parameters + "\r\n");
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

void Mode::Execute(Server &server, std::string message, int fd)
{
    Client *client = server.getClient(fd);
    if (Utils::split(message, " ").size() == 2)
        Message::send(fd, ":"+ server.getName()+ " 324 "+ client->getNickName() +" " + Utils::split(message, " ")[1]);
    else
    {
        modeInfo info = Mode::getMode(message);
        Channel *channel = server.getChannel(info.channel);
        if (!channel->getOperator(fd))
            throw ClientException::NotOperatorException(server.getName(), fd , client->getNickName() ,channel->getName());
        if (info.key == OPERATOR)
            execOperator(server, info);
        else if (info.key == BAN)
            execBan(server, info);
        else if (info.key == LIMIT)
            execLimit(server, info);
        else if (info.key == INVITE_ONLY)
            execInviteOnly(server, info);
        else if (info.key == KEY)
            execKey(server, info);
    }
}