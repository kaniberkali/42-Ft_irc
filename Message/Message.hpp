#ifndef MESSAGE_HPP
# define MESSAGE_HPP

#include <iostream>

class Message {
    public:
        static void send(int fd, std::string message);
        Message();
        ~Message();
};


#endif
