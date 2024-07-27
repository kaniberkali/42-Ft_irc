#include "Command.hpp"
#include "../Parser/Parser.hpp"
#include "../Logger/Logger.hpp"
#include "../Message/Message.hpp"

std::string Command::QUIT = "QUIT";
std::string Command::JOIN = "JOIN";
std::string Command::CAP_LS = "CAP LS";
std::string Command::PRIVMSG = "PRIVMSG";
std::string Command::PART = "PART";
std::string Command::WHO = "WHO";

std::string Command::getCommand(std::string message)
{
    if (message.rfind(QUIT, 0) == 0)
        return (QUIT);
    else if (message.rfind(JOIN, 0) == 0)
        return (JOIN);
    else if (message.rfind(CAP_LS, 0) == 0)
        return (CAP_LS);
    else if (message.rfind(PRIVMSG, 0) == 0)
        return (PRIVMSG);
    else if (message.rfind(PART , 0) == 0)
        return (PART);
    else if (message.rfind(WHO,0) == 0)
        return (WHO);
    return ("");
}

void Command::execCapLs(Server &server, std::string message, int fd)
{
    clientInfo info = Parser::connectionMessage(message);
    server.setClientInfo(fd, info);
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
        Message::send(fd, ":" + sender + " JOIN " + info.function + "\r\n");
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
        channel->removeClient(server.getClient(fd));
        Message::send(fd, ":" + sender + " PART " + info.function + "\r\n");
    }
    else
        Logger::Error("Channel " + info.function + " not found.");
}

void Command::execWho(Server &server, std::string message, int fd)
{
    std::string sender = server.getClient(fd)->getNickName();
    parseInfo info = Parser::parse(message);
    Logger::Info(sender + " who to " + info.function);
    Channel *channel = server.getChannel(info.function);
    Client *client = server.getClient(fd);
    if (channel != NULL)
    {
        std::vector<Client*> clients = channel->getClients();
        for (size_t i = 0; i < clients.size(); i++) {
            Message::send(fd, "352 " + client->getNickName() + " " + info.function + " " + clients[i]->getUserName() + " " + clients[i]->getHostName() + " " + clients[i]->getServerName() + " " + clients[i]->getNickName() + " H :0 " + clients[i]->getRealName() + "\r\n");
        }
        Message::send(fd, "315 " + client->getNickName() + " " + info.function + " :End of WHO list\r\n");
    }
    else
        Logger::Error("Channel " + info.function + " not found.");
}


void Command::Execute(Server &server, std::string message, int fd)
{
    std::string command = Command::getCommand(message);
    if (command == Command::CAP_LS)
        execCapLs(server, message, fd);
    else if (command == Command::QUIT)
        execQuit(server, fd);
    else if (command == Command::PRIVMSG)
        execPrivMsg(server, message, fd);
    else if (command == Command::JOIN)
        execJoin(server,message, fd);
    else if (command == Command::PART)
        execPart(server, message , fd);
    else if (command == Command::WHO)
        execWho(server, message, fd);

}
