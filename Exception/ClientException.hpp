#ifndef CLIENTEXCEPTION_HPP
# define CLIENTEXCEPTION_HPP

#include <iostream>

class ClientException : public std::exception {
    private:
        std::string _errorCode;
        std::string _message;
    public:
        class ASDException;
        ClientException(const std::string &errorCode, const std::string& msg) : _errorCode(errorCode), _message(msg) {}
        virtual ~ClientException() throw() {}
        virtual const char* what() const throw() { return _message.c_str(); }

        std::string getErrorCode() const {
            return _errorCode;
        }
};

class ClientException::ASDException : public ClientException {
public:
    ASDException() : ClientException("443","asd") {}
};

#endif