#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "../Parser/Parser.hpp"

class Client
{
    private:
        int _fd;
        bool _login;
        std::string _userName;
        std::string _hostName;
        std::string _serverName;
        std::string _realName;
        std::string _nickName;
        std::string _password;
    public:
        Client(int fd, clientInfo info);
        ~Client();
        bool isLogin() const;
        int getFd() const;
};

#endif