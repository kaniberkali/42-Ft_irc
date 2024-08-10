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
        bool _alreadyRegistered;
        std::string _userName;
        std::string _hostName;
        std::string _serverName;
        std::string _realName;
        std::string _nickName;
        std::string _password;
    public:
        Client(struct pollfd fd, std::string hostName);
        ~Client();
        bool isLogin() const;
        struct pollfd getFd() const;
        void login(serverInfo server, userInfo info);
        std::string getNickName();
        std::string getUserName();
        std::string getHostName();
        std::string getServerName();
        std::string getRealName();
        std::string getPassword();
        bool isAlreadyRegistered();

        void setNickName(std::string nickName);
        void setPassword(std::string name);
        void setUserName(std::string name);
        void setRealName(std::string name);
        void setAlreadyRegistered(bool status);
        userInfo getInfo();

};

#endif