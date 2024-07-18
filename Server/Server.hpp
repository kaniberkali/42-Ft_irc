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
# define BUFFER_SIZE 512

#include <vector>
#include <poll.h>
#include "../Client/Client.hpp"

class Server
{
    private:
        int _maxClients;
        int _port;
        std::string _password;
        static bool _terminate;
        int _socketFd;
        struct sockaddr_in _serverAddr;
        std::vector<struct pollfd> _fds;
        std::vector<Client *> _clients;

        std::vector<std::string> messages;


    public:
        Server(void);
        Server(int port, std::string password);
        Server(Server const &server);
        ~Server(void);
        Server const &operator=(Server const &server);
        static void quit(void);
        void init(void);
        void listen(void);
        void listen(int fd);
        void read();
        void newClient(int fd, clientInfo info);
        void removeClient(int fd);
        std::string read(int fd);
        void close(int fd);
};



#endif
