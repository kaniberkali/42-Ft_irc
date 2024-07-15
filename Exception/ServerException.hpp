#ifndef SERVEREXCEPTION_HPP
# define SERVEREXCEPTION_HPP

#include <iostream>

# define INVALID_PORT_RANGE "Port number is out of range"
# define PORT_NOT_DIGIT "Port number has to be include digit number"
# define SOCKET_NOT_CREATED "Socket fd is not created"
# define BINDING_FAILED "Binding process didn't work for socket fd between and address"
# define SOCKET_NOT_ACCEPT "Socket not accept"
# define SOCKET_LISTEN_EXCEPTION "Listen process didn't work for socket fd"

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
        ServerException(const std::string& msg = "") : _message(msg) {}
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



#endif
