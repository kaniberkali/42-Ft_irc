#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <netinet/in.h>

# define DEFAULT_PORT 6667
# define DEFAULT_PASSWORD "1111"

# define EXIT 1
# define SUCCESS 0

# define MIN_PORT 1 
# define MAX_PORT 65535

# define MAX_CLIENTS 44
# define BUFFER_SIZE 1024

class Server
{
    private:
        int _maxClients;
        int _port;
        std::string _password;
        bool _terminate;
        int _socketFd;
        struct sockaddr_in _serverAddr;
    public:
        Server(void);
        Server(int port, std::string password);
        Server(Server const &server);
        ~Server(void);
        Server const &operator=(Server const &server);
        int getPort(void) const;
        std::string getPassword(void) const;
        void setSettings(int port, std::string password, int maxClients);
        void quit(void);
        void initSocket(void);
};



#endif
