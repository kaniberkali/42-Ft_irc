
#ifndef EXCEPTION_HPP
# define EXCEPTION_HPP

# include <string>

class Exception {
    public:
        class Server : public std::exception {
            private:
                std::string _message;
            public:
                Server(const std::string& msg) : _message(msg) {}
                virtual ~Server() throw() {}

                virtual const char* what() const throw() {
                    return _message.c_str();
                }
        };

        class Client : public std::exception {
            private:
                std::string _errorCode;
                std::string _message;

            public:
                Client(const std::string &errorCode, const std::string& msg)
                        : _errorCode(errorCode), _message(msg) {}
                virtual ~Client() throw() {}

                virtual const char* what() const throw() {
                    return _message.c_str();
                }

                std::string getErrorCode() const {
                    return _errorCode;
                }
        };

};

#endif