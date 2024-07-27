#ifndef MESSAGE_HPP
# define MESSAGE_HPP

#include <iostream>
#include "../Channel/Channel.hpp"
#include "../Client/Client.hpp"

class Message {
    public:
        static void send(int fd, std::string message);
        static void send(Channel *channel, Client *client, std::string message);
        Message();
        ~Message();
};


#endif
