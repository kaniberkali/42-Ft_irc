#include "Client.hpp"
#include "../Logger/Logger.hpp"
#include "../Message/Message.hpp"
#include <netdb.h>
#include "../Exception/ClientException.hpp"
#include "../Utils/Utils.hpp"

Client::Client(struct pollfd fd, std::string hostName) : _fd(fd), _login(false), _hostName(hostName)
{
    _nickName = "*";
    _userName = "*";
    _serverName = "*";
    _realName = "*";
    _password = "*";
    _alreadyRegistered = false;
    Logger::Info("Client connected from " + _hostName, 0);
}

Client::~Client()
{
    Logger::Warning(_nickName + " disconnected from the server");
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

std::string Client::getPassword()
{
    return _password;
}

void Client::setNickName(std::string nickName)
{
    _nickName = nickName;
}

void Client::setPassword(std::string name)
{
    _password = name;
}

void Client::setRealName(std::string realName)
{
    _realName = realName;
}

void Client::setUserName(std::string userName)
{
    _userName = userName;
}


void Client::login(serverInfo server, userInfo user)
{
    if (server.password == user.password)
    {
        if (!_alreadyRegistered)
        {
            Message::send(_fd.fd,
                          ":" + server.name + " 001 " + user.nickName + " Welcome to the Internet Relay Network :" +
                          user.nickName + "!" + user.userName + "@" + user.hostName + "\r\n");
            Message::send(_fd.fd, ":" + server.name + " 002 " + user.nickName + " Your host is "+server.name +", running version " +
                                  server.version + "\r\n");
            Message::send(_fd.fd, ":" + server.name + " 003 " + user.nickName + " This server was created " +
                                  server.createDate + "\r\n");
            Message::send(_fd.fd, ":" + server.name + " 004 " + user.nickName + " " + server.name +" " + server.version + " Available user modes:  Available channel modes: biklot\r\n");
            _login = true;
        }
    }
    else
        throw ClientException::PasswordMismatchException(server.name, _fd.fd, user.nickName);
}

void Client::setAlreadyRegistered(bool status)
{
    _alreadyRegistered = status;
}

bool Client::isAlreadyRegistered()
{
    return _alreadyRegistered;
}

userInfo Client::getInfo()
{
    userInfo info;
    info.nickName = _nickName;
    info.password = _password;
    info.userName = _userName;
    info.realName = _realName;
    info.hostName = _hostName;
    return info;
}

