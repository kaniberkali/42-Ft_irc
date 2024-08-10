#include "Message.hpp"
#include "../Logger/Logger.hpp"
#include <sys/socket.h>
#include "../Utils/Utils.hpp"

Message::Message()
{
}

Message::~Message()
{
}

void Message::send(int fd, std::string message)
{
    int bytes_sent = ::send(fd, message.c_str(), message.length(), 0);
    if (bytes_sent < 0)
        Logger::Error("Send message failed");
    else
        Logger::HexChat(Utils::trim(message));
}

void Message::send(Channel *channel, Client *client, std::string message)
{
    std::vector<Client *> clients = channel->getClients();
    for (size_t i = 0; i < clients.size(); i++)
        if (clients[i]->getNickName() != client->getNickName())
            send(clients[i]->getFd().fd, message);
}