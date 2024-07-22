#include "Client.hpp"
#include "../Logger/Logger.hpp"
#include "../Message/Message.hpp"

Client::Client(struct pollfd fd) : _fd(fd), _login(false)
{

}

Client::~Client()
{
    Logger::Warning(_nickName + " disconnected from the server");
}

void Client::setInfo(clientInfo info)
{
    _userName = info.userName;
    _realName = info.realName;
    _nickName = info.nickName;
    _password = info.password;
    Logger::Info(_realName + " connected to the server with the name " + _nickName);
    Message::send(_fd.fd, "NICK " + _nickName + "\r\n");
    Message::send(_fd.fd, "USER " + _realName + " 0 * :" + _userName + "\r\n");
    Message::send(_fd.fd, "JOIN #test\r\n");
}

bool Client::isLogin() const
{
    return _login;
}

struct pollfd Client::getFd() const
{
    return _fd;
}


