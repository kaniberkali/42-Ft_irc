#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <iostream>
#include <vector>
#include "../Client/Client.hpp"

#define NON_LIMITED 0

class Channel
{
    private:
        std::string _name;
        std::vector<Client *> _clients;
        std::vector<Client *> _operators;
        std::vector<Client *> _bans;
        std::vector<Client *> _invites;
        size_t _limit;
        bool _inviteOnly;
        std::string _password;
        std::string _topic;
    public:
        void addClient(std::string serverName, Client *client);
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
        Client *getOperator(std::string nickName);

        void addBan(Client *client);
        void removeBan(Client *client);
        std::vector<Client *> getBans();
        Client *getBan(int fd);
        Client *getBan(std::string hostName);

        void addInvite(Client *client);
        void removeInvite(Client *client);
        std::vector<Client *> getInvites();
        Client *getInvite(int fd);
        Client *getInvite(std::string nickName);

        void setLimit(int limit);
        void setInviteOnly(bool inviteOnly);
        void setPassword(std::string password);
        void setTopic(std::string topic);
        std::string getTopic();
        std::string getPassword();
        std::string getModes();
        int getLimit();
};

#endif