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
    public:
        void addClient(Client *client);
        void removeClient(Client *client);
        std::vector<Client *> getClients();
        std::string getName();
        Channel(std::string name);
};

#endif