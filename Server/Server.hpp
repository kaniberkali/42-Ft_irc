#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <netinet/in.h>

# define DEFAULT_PORT 6667
# define DEFAULT_PASSWORD "1111"
# define DEFAULT_NAME "irc.ft_messenger.net"
# define DEFAULT_VERSION "ft_messenger-v1.0.0"

# define EXIT 1
# define SUCCESS 0

# define MIN_PORT 1 
# define MAX_PORT 65535

# define MAX_CLIENTS 44
# define BUFFER_SIZE 512

# define TIMEOUT 60000

#include <vector>
#include <poll.h>
#include "../Client/Client.hpp"
#include "../Channel/Channel.hpp"

class Server
{
    private:
        int _maxClients;
        int _port;
        std::string _password;
        static bool _terminate;
        struct pollfd _socketFd;
        struct sockaddr_in _serverAddr;
        std::vector<Client *> _clients;
        std::vector<Channel *> _channels;
        std::string _name;
        std::string _version;
        std::string _createdDate;
    public:
        Server(void);
        Server(int port, std::string password);
        Server(Server const &server);
        ~Server(void);
        Server const &operator=(Server const &server);
        static void quit(void);
        void init(void);
        void listen(void);
        void listen(int fd, std::string hostName);
        void read();
        void removeClient(int fd);
        std::string read(int fd);
        void close(int fd);
        Client *getClient(int fd);
        Client *getClient(std::string nickName);
        Client *getClientByNickName(std::string nickname);
        void addChannel(Channel *channel);
        void removeChannel(std::string name);
        Channel *getChannel(std::string name);
        std::string getName();
        std::string getVersion();
        Channel *getChannel(int index);
        std::vector<Channel *> getChannels();
        std::vector<Client *> getClients();
        std::string getCreateDate();
        serverInfo getInfo();
};



#endif
