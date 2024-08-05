#ifndef CLIENTEXCEPTION_HPP
# define CLIENTEXCEPTION_HPP

#include <iostream>
#include "../Logger/Logger.hpp"
#include "../Server/Server.hpp"
#include "../Message/Message.hpp"
#include "../Utils/Utils.hpp"

#define PASSWORD_INCORRECT(nickName) nickName + " :Password incorrect"
#define NOT_OPERATOR(nickName, channelName) nickName + " " + channelName + " :You're not channel operator"
#define NO_SUCH_NICK_OR_CHANNEL(nickName, channelName) nickName + " " + channelName + " :No such nick/channel"
#define BAN_CHANNEL(channelName) channelName + " :Cannot join channel (+b)"
#define CHANNEL_FULL(channelName) channelName + " :Cannot join channel (+l)"
#define ERR_INVITE_ONLY_CHAN(channelName) channelName + " :Cannot join channel (+i)"
#define WRONG_PASSWORD(channelName) channelName + " :Cannot join channel (+k)"

// :irc.ft_messenger.net 464 eymen :Password incorrect
// :irc.ft_messenger.net 482 ogdurkan #kanal_2 :You're not channel operator
class ClientException : public std::exception {
    private:
        int _errorCode;
        std::string _message;
    public:
        class NotOperatorException;
        class PasswordMismatchException;
        class NoSuchNickOrChannelException;
        class BanException;
        class ChannelFullException;
        class InviteOnlyException;
        class KeySetException;
        class WrongPasswordException;
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

class ClientException::NoSuchNickOrChannelException : public ClientException {
    public:
        NoSuchNickOrChannelException(std::string serverName, int fd, std::string nickName,std::string channelName)
         : ClientException(serverName, fd, 401, NO_SUCH_NICK_OR_CHANNEL(nickName, channelName)) { }
};

class ClientException::BanException : public ClientException {
    public:
        BanException(std::string serverName, int fd , std::string  channelName): ClientException(serverName, fd, 474, BAN_CHANNEL(channelName)) { }
};

class ClientException::ChannelFullException : public ClientException {
    public:
        ChannelFullException(std::string serverName, int fd , std::string  channelName) : ClientException(serverName, fd, 471, CHANNEL_FULL(channelName)) { }
};


 class ClientException::InviteOnlyException : public ClientException {
     public:
         InviteOnlyException(std::string serverName, int fd, std::string channelName) : ClientException(serverName, fd, 473,ERR_INVITE_ONLY_CHAN(channelName)) {}
 };

// kullanılmadı !
class ClientException::KeySetException : public ClientException {
    public:
        KeySetException(std::string serverName, int fd, std::string channelName) : ClientException(serverName, fd, 467,ERR_INVITE_ONLY_CHAN(channelName)) {}
};

class ClientException::WrongPasswordException : public ClientException {
    public:
        WrongPasswordException(std::string serverName, int fd, std::string channelName) : ClientException(serverName, fd, 475, WRONG_PASSWORD(channelName)) {}
};


#endif