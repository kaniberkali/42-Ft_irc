#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>

# define DEFAULT_PORT 6667
# define DEFAULT_PASS "1111"

# define EXIT 1
# define SUCCESS 0

# define MIN_PORT 1 
# define MAX_PORT 65535

class Server
{
    private:
        int _port;
        std::string _pass;
    public:
        Server(void);
        Server(int port, std::string pass);
        Server(Server const &server);
        ~Server(void);
        Server const &operator=(Server const &server);
};



#endif
