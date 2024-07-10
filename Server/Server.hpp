#ifndef SERVER_HPP
# define SERVER_HPP

# define DEFAULT_PORT 6667
# define DEFAULT_PASS "1111"

class Server
{
    private:
        int _port;
    public:
        Server(void);
        Server(int port);
        Server(Server const &server);
        ~Server(void);
        Server const &operator=(Server const &server);
};



#endif
