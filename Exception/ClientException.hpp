#ifndef CLIENTEXCEPTION_HPP
# define CLIENTEXCEPTION_HPP

#include <iostream>
#include "../Logger/Logger.hpp"
#include "../Server/Server.hpp"
#include "../Message/Message.hpp"
#include "../Utils/Utils.hpp"

#define PASSWORD_INCORRECT(nickName) nickName + " :Password incorrect"
#define NOT_OPERATOR(nickName, channelName) nickName + " " + channelName + " :You're not channel operator"

// :irc.ft_messenger.net 464 eymen :Password incorrect
// :irc.ft_messenger.net 482 ogdurkan #kanal_2 :You're not channel operator
class ClientException : public std::exception {
    private:
        int _errorCode;
        std::string _message;
    public:
        class NotOperatorException;
        class PasswordMismatchException;
        ClientException(std::string serverName, int fd, int errorCode, const std::string& msg) : _errorCode(errorCode), _message(msg)
        {
            Logger::Fatal(":" + serverName + " " + Utils::toString(_errorCode) + " " + _message);
            Message::send(fd, ":" + serverName + " " + Utils::toString(_errorCode) + " " + _message + "\r\n");
        }
        virtual ~ClientException() throw() {}
        virtual const char* what() const throw() { return _message.c_str(); }
};

class ClientException::PasswordMismatchException : public ClientException {
    public:
        PasswordMismatchException(std::string serverName, int fd, std::string nickName) :
        ClientException(serverName, fd, 464, PASSWORD_INCORRECT(nickName)) { }
};

class ClientException::NotOperatorException : public ClientException {
    public:
        NotOperatorException(std::string serverName, int fd, std::string nickName, std::string channelName)
        : ClientException(serverName, fd, 482, NOT_OPERATOR(nickName, channelName)) { }
};


#endif