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
# define ONLY_CHANNEL_EXCEPTION "It should only be the channel name and the channel name must comply with the standards."
# define ONLY_NICK_EXCEPTION "You should define correct nickname"
# define ONLY_NICK_CHANNEL_EXCEPTION "NICKNAME AND CHANNEL have to be use correctly"
# define ONLY_MESSAGE_EXCEPTION "Message format is not properly"
# define ONLY_NICK_MESSAGE_EXCEPTION "Nick and message must be given correctly"
# define ONLY_CHANNEL_MESSAGE_EXCEPTION "Channel and message formats not suitable for using"
# define ONLY_MODE_EXCEPTION "Mode format is not properly"
# define MORE_THAN_TWO_PARAMETER_EXCEPTION "Number of parameters is not valid"
# define ONLY_CHANNEL_OR_NICK_EXCEPTION "You must use channel or nickname"
# define FIRST_PARAM_CHANNEL_EXCEPTION "First parameter must be channel after command"
# define UNKNOWN_COMMAND_EXCEPTION "This command is not supported"
# define SET_SOCK_OPT_EXCEPTION "SetSockOpt process didn't work for socket fd"
# define FCNTL_EXCEPTION "Fcntl process didn't work for socket fd"

class ServerException : public std::exception 
{
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
        class OnlyChannelException;
        class OnlyNickNameException;
        class OnlyNickAndChannelNameException;
        class OnlyMessageException;
        class OnlyNickAndMessageException;
        class OnlyChannelAndMessageException;
        class OnlyModeException;
        class MoreThanTwoWordsException;
        class OnlyChannelOrNickNameException;
        class FirstParamIsChannel;
        class UnknownCommandException;
        class SetSockOptException;
        class FcntlException;

        ServerException(std::string msg) : _message(msg) 
        {
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

class  ServerException::OnlyChannelException:public ServerException
{
    public:
        OnlyChannelException() : ServerException(ONLY_CHANNEL_EXCEPTION) { }
};

class ServerException::OnlyNickNameException:public ServerException
{
    public:
        OnlyNickNameException() : ServerException(ONLY_NICK_EXCEPTION) { }
};

class ServerException::OnlyNickAndChannelNameException : public ServerException
{
    public:
        OnlyNickAndChannelNameException() : ServerException(ONLY_NICK_CHANNEL_EXCEPTION) { }
};

class ServerException::OnlyMessageException : public ServerException
{
    public:
        OnlyMessageException()  : ServerException(ONLY_MESSAGE_EXCEPTION) { }
};

class ServerException::OnlyNickAndMessageException : public ServerException
{
    public:
        OnlyNickAndMessageException()  : ServerException(ONLY_NICK_MESSAGE_EXCEPTION) { }
};


class ServerException::OnlyChannelAndMessageException : public ServerException
{
    public:
        OnlyChannelAndMessageException()  : ServerException(ONLY_CHANNEL_MESSAGE_EXCEPTION) { }
};

class ServerException::OnlyModeException : public ServerException
{
    public:
        OnlyModeException()  : ServerException(ONLY_MODE_EXCEPTION) { }
};

class ServerException::MoreThanTwoWordsException : public ServerException
{
    public:
        MoreThanTwoWordsException()  : ServerException(MORE_THAN_TWO_PARAMETER_EXCEPTION) { }
};

class ServerException::OnlyChannelOrNickNameException :  public ServerException
{
    public:
        OnlyChannelOrNickNameException()  : ServerException(ONLY_CHANNEL_OR_NICK_EXCEPTION) { }
};

class ServerException::FirstParamIsChannel : public ServerException
{
    public:
        FirstParamIsChannel()  : ServerException(FIRST_PARAM_CHANNEL_EXCEPTION) { }
};

class ServerException::UnknownCommandException : public ServerException
{
    public:
        UnknownCommandException() : ServerException(UNKNOWN_COMMAND_EXCEPTION) { }
};

class ServerException::SetSockOptException : public ServerException
{
    public:
        SetSockOptException() : ServerException(SET_SOCK_OPT_EXCEPTION) { }
};

class ServerException::FcntlException : public ServerException
{
    public:
        FcntlException() : ServerException(FCNTL_EXCEPTION) { }
};

#endif
