#ifndef SERVEREXCEPTION_HPP
# define SERVEREXCEPTION_HPP

#include <iostream>
#include "../Logger/Logger.hpp"

# define INVALID_PORT_RANGE "Port number is out of range"
# define PORT_NOT_DIGIT "Port number has to be include digit number"
# define SOCKET_NOT_CREATED "Socket fd is not created"
# define BINDING_FAILED "Binding process didn't work for socket fd between and address"
# define SOCKET_NOT_ACCEPT "Socket not accept"
# define SOCKET_LISTEN_EXCEPTION "Listen process didn't work for socket fd"
# define SOCKET_READ_EXCEPTION "Read process didn't work for socket fd"
# define ALREADY_OPERATOR "You have been already operator"
# define NOT_ALREADY_OPERATOR "You are not operator"
# define ALREADY_BAN "You have been already ban"
# define NOT_ALREADY_BAN "You are not ban"
# define BAN_EXCEPTION "You are banned for this channel"
# define ALREADY_INVITE_EXCEPTION "You are already invited."
# define NOT_ALREADY_INVITE_EXCEPTION "You are not already invited."

class ServerException : public std::exception {
    private:
        std::string _message;
    public:
        class PortRangeException;
        class PortDigitException;
        class SocketException;
        class BindException;
        class AcceptException;
        class ListenException;
        class ReadException;
        class InvalidPasswordException;
        class AlreadyOperatorException;
        class NotAlreadyOperatorException;
        class AlreadyBanException;
        class NotAlreadyBanException;
        class BanException;
        class AlreadyInviteException;
        class NotAlreadyInviteException;
        ServerException(std::string msg) : _message(msg) {
            Logger::Error(_message);
        }
        virtual ~ServerException() throw() {}
        virtual const char* what() const throw() { return _message.c_str(); }
};



class ServerException::PortRangeException : public ServerException {
    public:
        PortRangeException() : ServerException(INVALID_PORT_RANGE) {}
};

class ServerException::PortDigitException : public ServerException {
    public:
        PortDigitException() : ServerException(PORT_NOT_DIGIT) {}
};

class ServerException::SocketException : public ServerException {
    public:
        SocketException() : ServerException(SOCKET_NOT_CREATED) {}
};

class ServerException::BindException : public  ServerException {
    public:
        BindException() : ServerException(BINDING_FAILED) {}
};

class ServerException::AcceptException : public ServerException {
    public:
        AcceptException() : ServerException(SOCKET_NOT_ACCEPT) {}
};

class ServerException::ListenException : public ServerException {
    public:
        ListenException() : ServerException(SOCKET_LISTEN_EXCEPTION) {}
};

class ServerException::ReadException : public ServerException {
    public:
        ReadException() : ServerException(SOCKET_READ_EXCEPTION) {}
};


class ServerException::AlreadyOperatorException : public ServerException {
    public:
        AlreadyOperatorException() : ServerException(ALREADY_OPERATOR) {}
};

class ServerException::NotAlreadyOperatorException: public ServerException
{
    public:
        NotAlreadyOperatorException() : ServerException(NOT_ALREADY_OPERATOR) {}
};

class ServerException::AlreadyBanException : public ServerException {
public:
    AlreadyBanException() : ServerException(ALREADY_BAN) {}
};

class ServerException::NotAlreadyBanException: public ServerException
{
public:
    NotAlreadyBanException() : ServerException(NOT_ALREADY_BAN) {}
};

class ServerException::BanException: public ServerException
{
    public:
        BanException() : ServerException(BAN_EXCEPTION) { }
};

class ServerException::AlreadyInviteException: public ServerException
{
public:
    AlreadyInviteException() : ServerException(ALREADY_INVITE_EXCEPTION) { }
};

class ServerException::NotAlreadyInviteException: public ServerException
{
public:
    NotAlreadyInviteException() : ServerException(NOT_ALREADY_INVITE_EXCEPTION) { }
};


#endif
