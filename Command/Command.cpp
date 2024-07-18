#include "Command.hpp"
#include "../Parser/Parser.hpp"
#include "../Logger/Logger.hpp"

std::string Command::QUIT = "QUIT";
std::string Command::JOIN = "JOIN";
std::string Command::NICK = "NICK";
std::string Command::PASS = "PASS";
std::string Command::USER = "USER";
std::string Command::CAP_LS = "CAP LS";

std::string Command::getCommand(std::string message)
{
    if (message.rfind(QUIT, 0) == 0)
        return (QUIT);
    else if (message.rfind(JOIN, 0) == 0)
        return (JOIN);
    else if (message.rfind(NICK, 0) == 0)
        return (NICK);
    else if (message.rfind(PASS, 0) == 0)
        return (PASS);
    else if (message.rfind(USER, 0) == 0)
        return (USER);
    else if (message.rfind(CAP_LS, 0) == 0)
        return (CAP_LS);
    return ("");
}

void Command::execCapLs(Server &server, std::string message, int fd)
{
    clientInfo info = Parser::connectionMessage(message);
    server.newClient(fd, info);
}

void Command::execQuit(Server &server, int fd)
{
    server.removeClient(fd);
}

void Command::Perform(Server &server, std::string message, int fd)
{
    std::string command = Command::getCommand(message);
    Logger::Debug(command);
    if (command == Command::CAP_LS)
        execCapLs(server, message, fd);
    else if (command == Command::QUIT)
        execQuit(server, fd);
}
