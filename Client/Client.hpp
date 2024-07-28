#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <poll.h>
#include "../Parser/Parser.hpp"
#include <netdb.h>

class Client
{
    private:
        struct pollfd _fd;
        bool _login;
        std::string _userName;
        std::string _hostName;
        std::string _serverName;
        std::string _realName;
        std::string _nickName;
        std::string _password;
    public:
        Client(struct pollfd fd, std::string hostName);
        void setInfo(clientInfo info);
        ~Client();
        bool isLogin() const;
        struct pollfd getFd() const;
        std::string  getNickName();
        std::string  getUserName();
        std::string  getHostName();
        std::string  getServerName();
        std::string  getRealName();

        void setNickName(std::string nickName);

};

#endif