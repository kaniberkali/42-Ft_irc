#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <iostream>
#include <vector>
#include "../Client/Client.hpp"

class Channel
{
    private:
        std::string _name;
        std::vector<Client *> _clients;
        std::vector<Client *> _operators;
        std::vector<Client *> _bans;
        size_t _limit;
    public:
        void addClient(Client *client);
        Client* getClient(int fd);
        Client *getClient(std::string nickname);
        void removeClient(Client *client);
        std::vector<Client *> getClients();
        std::vector<Client *> getClients(std::string hostName);
        std::string getName();
        Channel(std::string name);
        void addOperator(Client *client);
        void removeOperator(Client *client);
        std::vector<Client *> getOperators();
        Client *getOperator(int fd);
        Client *getOperator(std::string nickname);

        void addBan(Client *client);
        void removeBan(Client *client);
        std::vector<Client *> getBans();
        Client *getBan(int fd);
        Client *getBan(std::string nickname);
};

#endif