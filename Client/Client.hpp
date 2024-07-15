#ifndef CLIENT_HPP
# define CLIENT_HPP

class Client
{
    private:
        int _clientFd;
        std::string _name;
        std::string _hostname;
        std::string _realName;
        std::string _serverName;
        std::string _nickname;
        std::string _password;
};

#endif