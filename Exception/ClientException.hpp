#ifndef CLIENTEXCEPTION_HPP
# define CLIENTEXCEPTION_HPP

#include <iostream>
#include "../Logger/Logger.hpp"

#define  ERR_PASSWD_MISMATCH ":Password incorrect"


class ClientException : public std::exception {
    private:
        std::string _errorCode;
        std::string _message;
    public:
        class ASDException;
        class PasswordMismatchException;
        ClientException(const std::string &errorCode, const std::string& msg) : _errorCode(errorCode), _message(msg) {
            Logger::Error(_errorCode + " " + _message);
        }
        virtual ~ClientException() throw() {}
        virtual const char* what() const throw() { return _message.c_str(); }

        std::string getErrorCode() const {
            return _errorCode;
        }
};

class ClientException::PasswordMismatchException : public ClientException {
public:
    PasswordMismatchException() : ClientException("464", ERR_PASSWD_MISMATCH) {}
};

class ClientException::ASDException : public ClientException {
public:
    ASDException() : ClientException("446",":invalid incorrect") {}
};

#endif