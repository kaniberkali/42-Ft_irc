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
   // std::string message = ":kaniberkali PRIVMSG " + _nickName +" :Deneme\r\n";
   // Message::send(4, message);
}

bool Client::isLogin() const
{
    return _login;
}

struct pollfd Client::getFd() const
{
    return _fd;
}

std::string Client::getNickName()
{
    return _nickName;
}

std::string Client::getUserName()
{
    return _userName;
}

std::string Client::getHostName()
{
    return _hostName;
}

std::string Client::getServerName()
{
    return _serverName;
}

std::string Client::getRealName()
{
    return _realName;
}