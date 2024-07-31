#include "Command.hpp"
#include "../Parser/Parser.hpp"
#include "../Logger/Logger.hpp"
#include "../Message/Message.hpp"
#include "../Exception/ClientException.hpp"
#include "../Server/Server.hpp"
#include "../Channel/Channel.hpp"
#include "../Utils/Utils.hpp"

std::string Command::QUIT = "QUIT";
std::string Command::JOIN = "JOIN";
std::string Command::PRIVMSG = "PRIVMSG";
std::string Command::PART = "PART";
std::string Command::WHO = "WHO";
std::string Command::NICK = "NICK";
std::string Command::USER = "USER";
std::string Command::PASS = "PASS";

std::string Command::getCommand(std::string message)
{
    if (message.rfind(QUIT, 0) == 0)
        return (QUIT);
    else if (message.rfind(JOIN, 0) == 0)
        return (JOIN);
    else if (message.rfind(PRIVMSG, 0) == 0)
        return (PRIVMSG);
    else if (message.rfind(PART , 0) == 0)
        return (PART);
    else if (message.rfind(WHO,0) == 0)
        return (WHO);
    else if (message.rfind(NICK,0) == 0)
        return (NICK);
    else if (message.rfind(USER,0) == 0)
        return (USER);
    if (message.find(" ") != std::string::npos)
        return (Utils::split(message, " ")[0]);
    return (message);
}

void Command::execQuit(Server &server, int fd)
{
    server.removeClient(fd);
}

void Command::execJoin(Server &server, std::string message, int fd)
{
    std::string sender = server.getClient(fd)->getNickName();
    parseInfo info = Parser::parse(message);
    Logger::Info(sender + " join to " + info.function);
    Channel* channel = server.getChannel(info.function);
    if (channel != NULL)
    {
        channel->addClient(server.getClient(fd));
        std::vector<Client *>clients = channel->getClients();
        for (size_t i = 0; i < channel->getClients().size(); i++)
        {
            int targetFd = clients[i]->getFd().fd;
            Message::send(targetFd, ":" + sender + " JOIN " + info.function + "\r\n");
        }
    }
    else
    {
        Channel *newChannel = new Channel(info.function);
        newChannel->addClient(server.getClient(fd));
        server.addChannel(newChannel);
        Message::send(fd, ":" + sender + " JOIN " + info.function + "\r\n");
    }
}

void Command::execPrivMsg(Server &server, std::string message, int fd)
{
    std::string sender = server.getClient(fd)->getNickName();
    reciveMessage info = Parser::privateMessage(message);
    Logger::Info("Message from " + sender + " to " + info.target + " : " + info.message);
    Client* targetClient = server.getClientByNickName(info.target);
    if (targetClient == NULL)
    {
        Channel* channel = server.getChannel(info.target);
        if (channel != NULL)
        {
            for (size_t i = 0; i < channel->getClients().size(); i++)
            {
                int targetFd = channel->getClients()[i]->getFd().fd;
                if (targetFd != fd)
                    Message::send(targetFd, ":" + sender + " PRIVMSG " + info.target + " :" + info.message + "\r\n");
            }
        }
        else
            Logger::Error("Target client " + info.target + " not found.");
    }
    else
    {
        int targetFd = targetClient->getFd().fd;
        Message::send(targetFd, ":" + sender + " PRIVMSG " + info.target + " :" + info.message + "\r\n");
    }
}

void Command::execPart(Server &server, std::string message, int fd)
{
    std::string sender = server.getClient(fd)->getNickName();
    parseInfo info = Parser::parse(message);
    Logger::Info(sender + " part from " + info.function);
    Channel* channel = server.getChannel(info.function);
    if (channel != NULL)
    {
        std::vector<Client *> clients = channel->getClients();
        for (size_t i = 0; i < clients.size(); i++)
        {
            int targetFd = clients[i]->getFd().fd;
            Message::send(targetFd, ":" + sender + " PART " + info.function + "\r\n");
        }
        channel->removeClient(server.getClient(fd));
    }
    else
        Logger::Error("Channel " + info.function + " not found.");
}

void Command::execWho(Server &server, std::string message, int fd)
{
    parseInfo info = Parser::parse(message);
    Channel *channel = server.getChannel(info.function);

    if (channel != NULL)
    {
        std::vector<Client*> clients = channel->getClients();
        std::string clientNames = "@";
        for (size_t i = 0; i < clients.size(); i++) {
            clientNames += clients[i]->getNickName();
            if (i != clients.size() - 1)
                clientNames += " ";
        }
        for (size_t i = 0; i < clients.size(); i++)
        {
            Client *client = clients[i];
            Message::send(client->getFd().fd, ":" + server.getName() + " 353 " + client->getNickName() + " = " +
                              channel->getName() + " : " + clientNames + "\r\n");
            Message::send(client->getFd().fd, ":" + server.getName() + " 366 " + client->getNickName() + " " +
                              channel->getName() + " :End of /NAMES list\r\n");
            Message::send(client->getFd().fd, ":" + server.getName() + " 324 " + client->getNickName() + " " +
                              channel->getName() + "+n \r\n");
            for (size_t q = 0; q < clients.size(); q++)
                Message::send(client->getFd().fd, ":" + server.getName() + " 352 " + client->getNickName() + " " +
                                  channel->getName() + " " + clients[q]->getUserName() + " " + clients[q]->getHostName() + " " + server.getName() + " " + clients[q]->getNickName() + " H :0 " + clients[q]->getRealName() + "\r\n");
            Message::send(client->getFd().fd, ":" + server.getName() + " 315 " + client->getNickName() + " " + channel->getName() + " :End of /WHO list\r\n");
        }
    }
    else
    {
        Message::send(fd, ":" + server.getName() + " 366 * :End of WHO list\r\n");
    }
}

void Command::execNick(Server &server, std::string message, int fd)
{
    std::string sender = server.getClient(fd)->getNickName();
    parseInfo info = Parser::parse(message);
    Message::send(fd, ":" + sender + " NICK " + info.function + "\r\n");
    Logger::Info(sender + " changed nickname to " + info.function);
    server.getClient(fd)->setNickName(info.function);

    std::vector<Client *> clients = server.getClients();
    for (size_t i = 0; i < clients.size(); i++)
    {
        int targetFd = clients[i]->getFd().fd;
        if (targetFd != fd)
        {
            Message::send(targetFd, ":" + sender + " NICK " + info.function + "\r\n");
            Logger::Info(sender + " changed nickname to " + info.function);
        }
    }
}

void Command::execPass(Server &server, std::string message, int fd)
{
    parseInfo parse = Parser::parse(message);

    Client *client = server.getClient(fd);
    Logger::Debug(parse.function);
    client->setPassword(parse.function);
}

void Command::execUser(Server &server, std::string message, int fd)
{
    userInfo userInfo = Parser::userParse(message);
    serverInfo serverInfo = server.getInfo();

    Client *client = server.getClient(fd);
    client->setUserName(userInfo.userName);
    client->setRealName(userInfo.realName);

    userInfo.realName = client->getRealName();
    userInfo.userName = client->getUserName();
    userInfo.nickName = client->getNickName();
    userInfo.password = client->getPassword();
    try
    {
        client->login(serverInfo, userInfo);
    }
    catch (ClientException &e)
    {
        Message::send(fd, ":" + serverInfo.name + " 464 * :Password incorrect\r\n");
        server.removeClient(fd);
    }
}

void Command::Execute(Server &server, std::string message, int fd)
{
    std::vector<std::string> words = Utils::split(message, "\r\n");
    for (size_t i = 0; i < words.size(); ++i) {
        if (words.size() == 1 || words[i].empty())
            break;
        Execute(server, words[i], fd);
        Logger::Info("Command Detected: " + words[i]);
    }
    std::string command = Command::getCommand(message);
    if (words.size() == 1) {
        if (command == Command::QUIT)
            execQuit(server, fd);
        else if (command == Command::PRIVMSG)
            execPrivMsg(server, message, fd);
        else if (command == Command::JOIN)
            execJoin(server, message, fd);
        else if (command == Command::PART)
            execPart(server, message, fd);
        else if (command == Command::WHO)
            execWho(server, message, fd);
        else if (command == Command::NICK)
            execNick(server, message, fd);
        else if (command == Command::USER)
            execUser(server, message, fd);
        else if (command == Command::PASS)
            execPass(server, message, fd);
        else
            Message::send(fd, server.getName() + " 421 * " + command + " :Unknown command\r\n");
    }
}
