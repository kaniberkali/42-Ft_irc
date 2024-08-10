#include "Command.hpp"
#include "../Parser/Parser.hpp"
#include "../Logger/Logger.hpp"
#include "../Message/Message.hpp"
#include "../Exception/ClientException.hpp"
#include "../Exception/ServerException.hpp"
#include "../Server/Server.hpp"
#include "../Channel/Channel.hpp"
#include "../Utils/Utils.hpp"
#include "../Mode/Mode.hpp"
#include "../Checker/Checker.hpp"

std::string Command::QUIT = "QUIT";
std::string Command::JOIN = "JOIN";
std::string Command::PRIVMSG = "PRIVMSG";
std::string Command::PART = "PART";
std::string Command::WHO = "WHO";
std::string Command::NICK = "NICK";
std::string Command::USER = "USER";
std::string Command::PASS = "PASS";
std::string Command::NOTICE = "NOTICE";
std::string Command::MODE = "MODE";
std::string Command::KICK = "KICK";
std::string Command::INVITE = "INVITE";
std::string Command::TOPIC = "TOPIC";
std::string Command::WHOIS = "WHOIS";

std::string Command::getCommand(std::string message)
{
    if (message.find(" ") == std::string::npos)
        return (message);
    message = Utils::split(message, " ")[0];
    return (message);
}

void Command::execQuit(Server &server, std::string message, int fd)
{
    std::vector<Channel *>channels = server.getChannels();
    for (size_t i = 0; i < channels.size(); i++)
    {
        Channel *channel = channels[i];
        if (channel->getClient(fd) != NULL)
        {
            std::vector < Client * > clients = channel->getClients();
            for (size_t i = 0; i < clients.size(); i++)
            {
                int targetFd = clients[i]->getFd().fd;
                if (targetFd != fd)
                {
                    message = Utils::split(message, ":")[1];
                    Message::send(targetFd, ":" + server.getClient(fd)->getNickName() + " QUIT :" + message + "\r\n");
                }
            }
            channel->removeClient(server.getClient(fd));
        }
    }
    server.removeClient(fd);
}

void Command::execJoin(Server &server, std::string message, int fd) {
    try
    {
        Client *sender = server.getClient(fd);
        parseInfo info = Parser::parse(message);
        Channel *channel = server.getChannel(info.function);
        if (channel != NULL)
        {
            if (channel->getPassword() != "" && channel->getPassword() != info.value)
                throw ClientException::WrongPasswordException(server.getName(), fd, channel->getName());
            channel->addClient(server.getName(), server.getClient(fd));
            std::vector < Client * > clients = channel->getClients();
            for (size_t i = 0; i < channel->getClients().size(); i++) 
            {
                int targetFd = clients[i]->getFd().fd;
                Logger::Info(sender->getNickName() + " join to " + info.function);
                if (channel->getOperator(targetFd) != NULL)
                    Message::send(targetFd, ":" + sender->getNickName() + "!" + sender->getUserName() + "@"+ sender->getHostName() +" JOIN " + info.function + "\r\n");
                else
                    Message::send(targetFd, ":" + sender->getNickName() + "!" + sender->getUserName() + "@unknown JOIN " + info.function + "\r\n");
            }
        }
        else
        {
            Channel *newChannel = new Channel(info.function);
            newChannel->addClient(server.getName(), server.getClient(fd));
            server.addChannel(newChannel);
            Logger::Info(sender->getNickName() + " join to " + info.function);
            Message::send(fd, ":" + sender->getNickName() + "!" + sender->getUserName() + "@"+ sender->getHostName() +" JOIN " + info.function + "\r\n");
        }
    }
    catch (ClientException::BanException &e) { }
    catch (ClientException::ChannelFullException &e) { }
    catch (ClientException::InviteOnlyException &e) { }
    catch (ClientException::WrongPasswordException &e) { }
}

void Command::execPrivMsg(Server &server, std::string message, int fd) {
    std::string sender = server.getClient(fd)->getNickName();
    reciveMessage info = Parser::privateMessage(message);
    Logger::Info("Message from " + sender + " to " + Utils::trim(info.target) + " : " + info.message);
    Client *targetClient = server.getClientByNickName(info.target);
    if (targetClient == NULL) {
        Channel *channel = server.getChannel(info.target);
        if (channel != NULL) {
            for (size_t i = 0; i < channel->getClients().size(); i++) {
                int targetFd = channel->getClients()[i]->getFd().fd;
                if (targetFd != fd)
                    Message::send(targetFd, ":" + sender + " PRIVMSG " + info.target + " :" + info.message + "\r\n");
            }
        } else
            Logger::Error("Target client " + Utils::trim(info.target) + " not found.");
    } else {
        int targetFd = targetClient->getFd().fd;
        Message::send(targetFd, ":" + sender + " PRIVMSG " + info.target + " :" + info.message + "\r\n");
    }
}

void Command::execPart(Server &server, std::string message, int fd) {
    std::string sender = server.getClient(fd)->getNickName();
    parseInfo info = Parser::parse(message);
    Logger::Info(sender + " part from " + info.function);
    Channel *channel = server.getChannel(info.function);
    if (channel != NULL) {
        std::vector < Client * > clients = channel->getClients();
        for (size_t i = 0; i < clients.size(); i++) {
            int targetFd = clients[i]->getFd().fd;
            Message::send(targetFd, ":" + sender + " PART " + info.function + "\r\n");
        }
        channel->removeClient(server.getClient(fd));
    } else
        Logger::Error("Channel " + info.function + " not found.");
}

void Command::execWho(Server &server, std::string message, int fd)
{
    parseInfo info = Parser::parse(message);
    Channel *channel = server.getChannel(info.function);

    if (channel != NULL)
    {
        bool isOperator = channel->getOperator(fd) != NULL;
        std::vector < Client * > clients = channel->getClients();
        std::string clientNames = "";
        for (size_t i = 0; i < clients.size(); i++)
        {
            if (channel->getOperator(clients[i]->getFd().fd) != NULL)
                clientNames += "@";
            clientNames += clients[i]->getNickName();
            if (i != clients.size() - 1)
                clientNames += " ";
        }
        Client *client = channel->getClient(fd);
        Message::send(client->getFd().fd, ":" + server.getName() + " 353 " + client->getNickName() + " = " +
                                          channel->getName() + " : " + clientNames + "\r\n");
        Message::send(client->getFd().fd, ":" + server.getName() + " 366 " + client->getNickName() + " " +
                                          channel->getName() + " :End of /NAMES list\r\n");
        Message::send(client->getFd().fd, ":" + server.getName() + " 324 " + client->getNickName() + " " +
                                          channel->getName() + channel->getModes() +" \r\n");
        for (size_t q = 0; q < clients.size(); q++)
        {
            Message::send(client->getFd().fd, ":" + server.getName() + " 352 " + client->getNickName() + " " +
                                              channel->getName() + " " + clients[q]->getUserName() + " " + ((isOperator || clients[q] == client) ? clients[q]->getHostName() : "unknown") + " " + server.getName() + " " + clients[q]->getNickName() + " H :0 " + clients[q]->getRealName() + "\r\n");
        }
        Message::send(client->getFd().fd,
                      ":" + server.getName() + " 315 " + client->getNickName() + " " + channel->getName() +
                      " :End of /WHO list\r\n");
    }
    else
        Message::send(fd, ":" + server.getName() + " 366 * :End of WHO list\r\n");
}

void Command::execNotice(Server &server, std::string message, int fd) {
    Client *sender = server.getClient(fd);
    reciveMessage info = Parser::privateMessage(message);
    Logger::Info("Notice from " + sender->getNickName() + " to " + info.target + " : " + info.message);
    Client *targetClient = server.getClientByNickName(info.target);
    if (targetClient == NULL) {
        Channel *channel = server.getChannel(info.target);
        if (channel != NULL) {
            for (size_t i = 0; i < channel->getClients().size(); i++) {
                int targetFd = channel->getClients()[i]->getFd().fd;
                if (targetFd != fd)
                    Message::send(targetFd,
                                  ":" + sender->getNickName() + "!" + sender->getUserName() + "@0" + " NOTICE " +
                                  info.target + " :" + info.message + "\r\n");
            }
        } else
            Logger::Error("Target client " + Utils::trim(info.target) + " not found.");
    } else {
        int targetFd = targetClient->getFd().fd;
        if (targetFd != fd)
            Message::send(targetFd,
                          ":" + sender->getNickName() + "!" + sender->getUserName() + "@0" + " NOTICE " + info.target +
                          " :" + info.message + "\r\n");
    }
}

void Command::execNick(Server &server, std::string message, int fd)
{
    std::string sender = server.getClient(fd)->getNickName();
    parseInfo info = Parser::parse(message);
    Client *client = server.getClient(fd);
    try
    {
        if (server.getClientByNickName(info.function) != NULL)
            throw ClientException::NicknameInUseException(server.getName(), fd, info.function);

        if (sender == "*")
            Logger::Info("Set nickname to " + info.function);
        else
            Logger::Info(sender + " changed nickname to " + info.function);

        client->setNickName(info.function);

        if (sender != "*") {
            std::vector < Client * > clients = server.getClients();
            for (size_t i = 0; i < clients.size(); i++) {
                int targetFd = clients[i]->getFd().fd;
                if (targetFd != fd)
                    Message::send(targetFd, ":" + sender + " NICK " + info.function + "\r\n");
            }
        }

        if (client->isAlreadyRegistered())
        {
            client->setNickName(info.function);
            client->setAlreadyRegistered(false);
            client->login(server.getInfo(), client->getInfo());
        }
    }
    catch (ClientException::NicknameInUseException &e)
    {
        client->setAlreadyRegistered(true);
    }
}

void Command::execPass(Server &server, std::string message, int fd) {
    parseInfo parse = Parser::parse(message);

    Client *client = server.getClient(fd);
    client->setPassword(parse.function);
}

void Command::execUser(Server &server, std::string message, int fd) {
    userInfo userInfo = Parser::userParse(message);
    serverInfo serverInfo = server.getInfo();

    Client *client = server.getClient(fd);
    client->setUserName(userInfo.userName);
    client->setRealName(userInfo.realName);

    userInfo.realName = client->getRealName();
    userInfo.userName = client->getUserName();
    userInfo.nickName = client->getNickName();
    userInfo.password = client->getPassword();
    userInfo.hostName = client->getHostName();
    try
    {
        client->login(serverInfo, userInfo);
    }
    catch (ClientException::PasswordMismatchException &e) {
        server.removeClient(fd);
    }
}

void Command::execMode(Server &server, std::string message, int fd) {
    try
    {
        Mode::Execute(server, message, fd);
    }
    catch (ClientException::NotOperatorException &e) {}
}

void Command::execKick(Server &server, std::string message, int fd)
{
    parseInfo info = Parser::parse(message);
    std::string reason = "";
    if (message.find(":") != std::string::npos)
    {
        std::vector<std::string> words = Utils::split(message, ":");
        message = Utils::trim(words[0]);
        reason = Utils::trim(words[1]);
        info = Parser::parse(message);
    }

    Channel *channel = server.getChannel(info.function);
    try
    {
        if (channel->getOperator(fd) == NULL)
            throw ClientException::NotOperatorException(server.getName(), fd, server.getClient(fd)->getNickName(),info.function);
        if (channel->getClient(info.value) == NULL)
            throw ClientException::NoSuchNickOrChannelException(server.getName(), fd , info.value, channel->getName());
        std::vector < Client * > clients = channel->getClients();
        for (size_t i = 0; i < clients.size(); i++)
        {
            int targetFd = clients[i]->getFd().fd;
            if (!reason.empty())
                Message::send(targetFd, ":" + server.getName() + " KICK " + channel->getName() + " " + info.value + " :" + reason + "\r\n");
            else
                Message::send(targetFd, ":" + server.getName() + " KICK " + channel->getName() + " " + info.value + "\r\n");
        }
        channel->removeClient(channel->getClient(info.value));
    }
    catch (ClientException::NotOperatorException &e) {}
    catch (ClientException::NoSuchNickOrChannelException &e) {}
}

void Command::execInvite(Server &server, std::string message, int fd)
{
    parseInfo info = Parser::parse(message);
    Channel *channel = server.getChannel(info.value);
    try
    {
        if (!channel)
            throw ClientException::NoSuchNickOrChannelException(server.getName(), fd, info.value, "");
        if (channel->getOperator(fd) == NULL)
            throw ClientException::NotOperatorException(server.getName(), fd, server.getClient(fd)->getNickName(),info.value);
        if (server.getClient(info.function) == NULL)
            throw ClientException::NoSuchNickOrChannelException(server.getName(), fd , info.function, channel->getName());
        channel->addInvite(server.getClient(info.function));
        std::vector <Client *> clients = channel->getClients();
        for (size_t i = 0; i < clients.size(); i++)
        {
            int targetFd = clients[i]->getFd().fd;
            Message::send(targetFd, ":" + server.getName() + " INVITE " + channel->getName() + " " + info.value + "\r\n");
        }
    }
    catch (ClientException::NotOperatorException &e) { }
    catch (ClientException::NoSuchNickOrChannelException &e) {}
}

void Command::execTopic(Server &server, std::string message, int fd)
{
    parseInfo info = Parser::parse(message);
    Channel *channel = server.getChannel(info.function);
    try
    {
        if (channel->getOperator(fd) == NULL)
            throw ClientException::NotOperatorException(server.getName(), fd, server.getClient(fd)->getNickName(),info.function);
        if (info.value.empty())
        {
            Message::send(fd, ":" + server.getName() + " 332 " + server.getClient(fd)->getNickName() + " " + channel->getName() + " :" + channel->getTopic() + "\r\n");
            return;
        }
        info.value = Utils::split(message, ":")[1];
        channel->setTopic(info.value);
        std::vector < Client * > clients = channel->getClients();
        for (size_t i = 0; i < clients.size(); i++)
        {
            int targetFd = clients[i]->getFd().fd;
            Message::send(targetFd, ":" + server.getName() + " TOPIC " + channel->getName() + " :" + info.value + "\r\n");
        }
    }
    catch (ClientException::NotOperatorException &e) {}
}

void Command::execWhois(Server &server, std::string message, int fd)
{
    parseInfo info = Parser::parse(message);
    Client *client = server.getClient(fd);
    Client *targetClient = server.getClientByNickName(info.function);
    if (targetClient == NULL)
    {
        Channel *channel = server.getChannel(info.function);
        if (channel != NULL)
        {
            bool isOperator = channel->getOperator(fd) != NULL;
            std::vector <Client *> clients = channel->getClients();
            for (size_t i = 0; i < clients.size(); i++)
            {
                Message::send(client->getFd().fd, ":" + server.getName() + " 311 " + client->getNickName() + " " +
                                                  clients[i]->getNickName() + " " + clients[i]->getUserName() + " " +
                        ((isOperator || clients[i]->getFd().fd == fd) ? clients[i]->getHostName() : "unknown")  + " * :" + clients[i]->getRealName() +
                                                  "\r\n");
            }
            Message::send(client->getFd().fd, ":" + server.getName() + " 318 " + client->getNickName() + " " + info.function + " :End of /WHOIS list\r\n");
        }
        else
            Message::send(fd, ":" + server.getName() + " 401 " + client->getNickName() + " " + info.function + " :No such nick/channel\r\n");
    }
    else
    {
        std::vector<Channel *> channels = server.getChannels();
        for (size_t i = 0; i < channels.size(); i++)
        {
            Channel *channel = channels[i];
            if (channel->getClient(targetClient->getFd().fd) != NULL)
            {
                Message::send(client->getFd().fd, ":" + server.getName() + " 311 " + client->getNickName() + " " +
                                                  targetClient->getNickName() + " " + targetClient->getUserName() + " " +
                        ((channel->getOperator(fd) != NULL || targetClient->getFd().fd == fd) ? targetClient->getHostName() : "unknown") + " * :" + targetClient->getRealName() +
                                                  "\r\n");
                Message::send(client->getFd().fd, ":" + server.getName() + " 318 " + client->getNickName() + " " + info.function + " :End of /WHOIS list\r\n");
                return;
            }
        }
    }
}

void Command::Execute(Server &server, std::string message, int fd) {
    try
    {
        std::vector <std::string> words = Utils::split(message, "\r\n");
        for (size_t i = 0; i < words.size(); ++i)
        {
            if (words.size() == 1 || words[i].empty())
                break;
            Execute(server, words[i], fd);
        }
        std::string command = Command::getCommand(message);
        command = Utils::toUpper(command);
        if (words.size() == 1 && Checker::check(server, message, fd))
        {
            Client *client = server.getClient(fd);
            if (command == Command::QUIT)
                execQuit(server, message, fd);
            else if (command == Command::USER)
                execUser(server, message, fd);
            else if (command == Command::NICK)
                execNick(server, message, fd);
            else if (command == Command::PASS)
                execPass(server, message, fd);
            else if (client->isLogin())
            {
                if (command == Command::PRIVMSG)
                    execPrivMsg(server, message, fd);
                else if (command == Command::JOIN)
                    execJoin(server, message, fd);
                else if (command == Command::PART)
                    execPart(server, message, fd);
                else if (command == Command::WHO)
                    execWho(server, message, fd);
                else if (command == Command::NOTICE)
                    execNotice(server, message, fd);
                else if (command == Command::MODE)
                    execMode(server, message, fd);
                else if (command == Command::KICK)
                    execKick(server, message, fd);
                else if (command == Command::INVITE)
                    execInvite(server, message, fd);
                else if (command == Command::TOPIC)
                    execTopic(server, message, fd);
                else if (command == Command::WHOIS)
                    execWhois(server, message, fd);
                else
                    throw ClientException::UnknownCommandException(server.getName(), fd, Utils::trim(command),server.getClient(fd)->getNickName());
            }
        }
    }
    catch (ClientException::UnknownCommandException &e) { }
    catch (ServerException::OnlyChannelException &e) { }
    catch (ServerException::OnlyNickNameException &e) { }
    catch (ServerException::OnlyNickAndChannelNameException &e) { }
    catch (ServerException::OnlyMessageException &e) { }
    catch (ServerException::OnlyNickAndMessageException &e) { }
    catch (ServerException::OnlyChannelAndMessageException &e) { }
    catch (ServerException::OnlyModeException &e) { }
    catch (ServerException::MoreThanTwoWordsException &e) { }
    catch (ServerException::OnlyChannelOrNickNameException &e) { }
    catch (ServerException::FirstParamIsChannel &e) { }
    catch (ServerException::UnknownCommandException &e) { }
}
