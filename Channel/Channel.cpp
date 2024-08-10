#include "Channel.hpp"
#include "../Exception/ServerException.hpp"
#include "../Exception/ClientException.hpp"
#include "../Utils/Utils.hpp"

void Channel::addClient(std::string serverName, Client *client)
{
    if (getBan(client->getHostName()) != NULL)
        throw ClientException::BanException(serverName, client->getFd().fd, this->_name);
    if (_limit != 0 && _clients.size() == _limit)
        throw ClientException::ChannelFullException(serverName, client->getFd().fd, this->_name);
    if (_inviteOnly && getInvite(client->getNickName()) == NULL)
        throw ClientException::InviteOnlyException(serverName, client->getFd().fd, this->_name);
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
            try
            {
                if (getOperator(client->getNickName()) != NULL)
                    removeOperator(_clients[i]);
            }
            catch (ServerException::NotAlreadyOperatorException &e) { }
            _clients.erase(_clients.begin() + i);
            break;
        }
    }
}

std::string Channel::getName()
{
    return _name;
}

Channel::Channel(std::string name) : _name(name), _limit(NON_LIMITED), _inviteOnly(false), _password("")
{

}

std::vector<Client *> Channel::getClients()
{
    return _clients;
}

std::vector<Client *> Channel::getClients(std::string hostName)
{
    std::vector<Client *> clients;
    for (size_t i = 0; i < _clients.size(); i++)
    {
        if (_clients[i]->getHostName() == hostName)
            clients.push_back(_clients[i]);
    }
    return clients;
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

void Channel::addBan(Client *client)
{
    for (size_t i = 0; i < _bans.size(); i++)
    {
        if (_bans[i]->getHostName() == client->getHostName())
            throw ServerException::AlreadyBanException();
    }
    _bans.push_back(client);
    removeClient(client);
}

void Channel::removeBan(Client *client)
{
    bool isBaned = false;
    for (size_t i = 0; i < _bans.size(); i++)
    {
        if (_bans[i]->getHostName() == client->getHostName())
        {
            _bans.erase(_bans.begin() + i);
            isBaned = true;
            break;
        }
    }
    if (!isBaned)
        throw ServerException::NotAlreadyBanException();
}

std::vector<Client *> Channel::getBans()
{
    return _bans;
}

Client *Channel::getBan(int fd)
{
    for (size_t i = 0; i < _bans.size(); i++)
    {
        if (_bans[i]->getFd().fd == fd)
            return _bans[i];
    }
    return NULL;
}

Client *Channel::getBan(std::string hostName)
{
    for (size_t i = 0; i < _bans.size(); i++)
    {
        if (_bans[i]->getHostName() == hostName)
            return _bans[i];
    }
    return NULL;
}

void Channel::setLimit(int limit)
{
    _limit = limit;
}

void Channel::setInviteOnly(bool inviteOnly)
{
    _inviteOnly = inviteOnly;
}

void Channel::addInvite(Client *client)
{
    for (size_t i = 0; i < _invites.size(); i++)
    {
        if (_invites[i] == client)
            throw ServerException::AlreadyInviteException();
    }
    _invites.push_back(client);
}

void Channel::removeInvite(Client *client)
{
    bool isDeleted = false;
    for (size_t i = 0; i < _invites.size(); i++)
    {
        if (_invites[i] == client)
        {
            _invites.erase(_invites.begin() + i);
            isDeleted = true;
            break;
        }
    }
    if (!isDeleted)
        throw ServerException::NotAlreadyInviteException();
}

std::vector<Client *> Channel::getInvites()
{
    return _invites;
}

Client *Channel::getInvite(int fd)
{
    for (size_t i = 0; i < _invites.size(); i++)
    {
        if (_invites[i]->getFd().fd == fd)
            return _invites[i];
    }
    return NULL;
}

Client *Channel::getInvite(std::string nickname)
{
    for (size_t i = 0; i < _invites.size(); i++)
    {
        if (_invites[i]->getNickName() == nickname)
            return _invites[i];
    }
    return NULL;
}

void Channel::setPassword(std::string password)
{
    _password = password;
}

std::string Channel::getPassword()
{
    return _password;
}

void Channel::setTopic(std::string topic)
{
    _topic = topic;
}

std::string Channel::getTopic()
{
    return _topic;
}

std::string Channel::getModes()
{
    std::string modes = "";

    if (_password != "")
        modes += " +k " + _password;
    if (_limit != 0)
        modes += " +l " + Utils::toString((int)_limit);
    if (_inviteOnly)
        modes += " +i";
    return modes;
}

int Channel::getLimit()
{
    return _limit;
}