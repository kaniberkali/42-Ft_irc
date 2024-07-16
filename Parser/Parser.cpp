#include "Parser.hpp"
#include "../Logger/Logger.hpp"
clientInfo Parser::connectionMessage(std::string message) {
    clientInfo info;
    Logger::Debug("Parsing connection message: " + message);
    return info;
}