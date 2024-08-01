#include "Channel.hpp"
#include "../Exception/ServerException.hpp"

void Channel::addClient(Client *client)
{
    if (_clients.size() == 0)
        _operators.push_back(client);
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

Client* Channel::getClient(int fd)
{
    for (size_t i = 0; i < _clients.size(); i++)
    {
        if (_clients[i]->getFd().fd == fd)
            return _clients[i];
    }
    return NULL;
}

Client *Channel::getClient(std::string nickname)
{
    for (size_t i = 0; i < _clients.size(); i++)
    {
        if (_clients[i]->getNickName() == nickname)
            return _clients[i];
    }
    return NULL;
}

void Channel::addOperator(Client *client)
{
    bool isOperator = false;
    for (size_t i = 0; i < _operators.size(); i++)
    {
        if (_operators[i] == client)
        {
            isOperator = true;
            break;
        }
    }
    if (!isOperator)
        _operators.push_back(client);
    else
        throw ServerException::AlreadyOperatorException();
}

void Channel::removeOperator(Client *client)
{
    bool isDeleted = false;
    for (size_t i = 0; i < _operators.size(); i++)
    {
        if (_operators[i] == client)
        {
            _operators.erase(_operators.begin() + i);
            isDeleted = true;
            break;
        }
    }
    if (!isDeleted)
        throw ServerException::NotAlreadyOperatorException();
}

std::vector<Client *> Channel::getOperators()
{
    return _operators;
}

Client *Channel::getOperator(int fd)
{
    for (size_t i = 0; i < _operators.size(); i++)
    {
        if (_operators[i]->getFd().fd == fd)
            return _operators[i];
    }
    return NULL;
}

Client *Channel::getOperator(std::string nickname)
{
    for (size_t i = 0; i < _operators.size(); i++)
    {
        if (_operators[i]->getNickName() == nickname)
            return _operators[i];
    }
    return NULL;
}