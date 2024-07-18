#include "Client.hpp"

Client::Client(int fd , clientInfo info) : _fd(fd), _login(false)
{
    _userName = info.userName;
    _realName = info.realName;
    _nickName = info.nickName;
    _password = info.password;
}

Client::~Client()
{

}

bool Client::isLogin() const
{
    return _login;
}

int Client::getFd() const
{
    return _fd;
}

