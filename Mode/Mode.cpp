#include "Mode.hpp"
#include "../Exception/ClientException.hpp"
#include "../Exception/ServerException.hpp"
#include "../Message/Message.hpp"
#include "../Utils/Utils.hpp"

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

void execOperator(Server &server, modeInfo info, int fd)
{
    Channel *channel = server.getChannel(info.channel);
    Client *client = channel->getClient(info.parameters);
    std::vector<Client *> clients = channel->getClients();
    fd = fd + 1;
    try {
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
            execOperator(server, info, fd);
    }
}