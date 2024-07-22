#include "Message.hpp"
#include "../Logger/Logger.hpp"
#include <sys/socket.h>

Message::Message()
{
}

Message::~Message()
{
}

void Message::send(int fd, std::string message)
{
    int bytes_sent = ::send(fd, message.c_str(), message.length(), MSG_NOSIGNAL);
    if (bytes_sent < 0)
        Logger::Error("Send message failed");
    else
        Logger::Info("Message sended");
}