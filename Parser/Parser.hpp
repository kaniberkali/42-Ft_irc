#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <sstream>

struct clientInfo {
    std::string userName;
    std::string password;
    std::string nickName;
    std::string realName;
};

class Parser
{
    public:
        static clientInfo connectionMessage(std::string message);
};

#endif
