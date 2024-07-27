#include "Channel.hpp"

void Channel::addClient(Client *client)
{
    _clients.push_back(client);
}

void Channel::removeClient(Client *client)
{
    for (size_t i = 0; i < _clients.size(); i++)
    {
        if (_clients[i] == client)
        {
            _clients.erase(_clients.begin() + i);
            break;
        }
    }
}

std::string Channel::getName()
{
    return _name;
}

Channel::Channel(std::string name) : _name(name)
{

}

std::vector<Client *> Channel::getClients()
{
    return _clients;
}