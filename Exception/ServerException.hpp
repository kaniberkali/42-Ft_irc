#ifndef SERVEREXCEPTION_HPP
# define SERVEREXCEPTION_HPP

#include <iostream>

class ServerException : public std::exception {
    private:
        std::string _message;
    public:
        class PortRangeException;
        class InvalidPortException;
        class InvalidIPException;
        class InvalidSocketException;
        class InvalidBindException;
        class InvalidListenException;
        class SocketException;
        class BindException;
        class ListenException;
        class AcceptException;
        class ReceiveException;
        ServerException(const std::string& msg = "") : _message(msg) {}
        virtual ~ServerException() throw() {}
        virtual const char* what() const throw() { return _message.c_str(); }
};

class ServerException::PortRangeException : public ServerException {
public:
    PortRangeException() : ServerException("Port Number is Out of Range") {}
};

class ServerException::InvalidPortException : public ServerException {
public:
    InvalidPortException() : ServerException("Port Number is Invalid") {}
};

class ServerException::InvalidIPException : public ServerException {
public:
    InvalidIPException() : ServerException("IP Address is Invalid") {}
};

class ServerException::InvalidSocketException : public ServerException {
public:
    InvalidSocketException() : ServerException("Socket is Invalid") {}
};

class ServerException::InvalidBindException : public ServerException {
public:
    InvalidBindException() : ServerException("Bind is Invalid") {}
};

class ServerException::InvalidListenException : public ServerException {
public:
    InvalidListenException() : ServerException("Listen is Invalid") {}
};

class ServerException::SocketException : public ServerException {
public:
    SocketException() : ServerException("Socket is Invalid") {}
};

class ServerException::BindException : public ServerException {
public:
    BindException() : ServerException("Bind is Invalid") {}
};

class ServerException::ListenException : public ServerException {
public:
    ListenException() : ServerException("Listen is Invalid") {}
};

class ServerException::AcceptException : public ServerException {
public:
    AcceptException() : ServerException("Accept is Invalid") {}
};

class ServerException::ReceiveException : public ServerException {
public:
    ReceiveException() : ServerException("Receive is Invalid") {}
};

#endif
