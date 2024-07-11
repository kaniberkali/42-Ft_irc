#include "Logger.hpp"
#include "../Utils/Utils.hpp"

const std::string Logger::FATAL_COLOR = "\033[1;31m";
const std::string Logger::ERROR_COLOR = "\033[0;31m";
const std::string Logger::WARNING_COLOR = "\033[0;33m";
const std::string Logger::INFO_COLOR = "\033[0;34m";
const std::string Logger::DEBUG_COLOR = "\033[0;32m";
const std::string Logger::TRACE_COLOR = "\033[0;37m";
const std::string Logger::END_COLOR = "\033[0m";
const int Logger::FATAL = 0;
const int Logger::ERROR = 1;
const int Logger::WARNING = 2;
const int Logger::INFO = 3;
const int Logger::DEBUG = 4;
const int Logger::TRACE = 5;

void Logger::Log(std::string message, int level)
{
    switch (level)
    {
        case Logger::FATAL:
            std::cout << Logger::FATAL_COLOR << Utils::time() << " [FATAL] " << message << Logger::END_COLOR << std::endl;
            break;
        case Logger::ERROR:
            std::cout << Logger::ERROR_COLOR << Utils::time() << " [ERROR] " << message << Logger::END_COLOR << std::endl;
            break;
        case Logger::WARNING:
            std::cout << Logger::WARNING_COLOR << Utils::time() << " [WARNING] " << message << Logger::END_COLOR << std::endl;
            break;
        case Logger::INFO:
            std::cout << Logger::INFO_COLOR << Utils::time() << " [INFO] " << message << Logger::END_COLOR << std::endl;
            break;
        case Logger::DEBUG:
            std::cout << Logger::DEBUG_COLOR << Utils::time() << " [DEBUG] " << message << Logger::END_COLOR << std::endl;
            break;
        case Logger::TRACE:
            std::cout << Logger::TRACE_COLOR << Utils::time() << " [TRACE] " << message << Logger::END_COLOR << std::endl;
            break;
    }
}

void Logger::Fatal(std::string message)
{
    Logger::Log(message, Logger::FATAL);
}

void Logger::Error(std::string message)
{
    Logger::Log(message, Logger::ERROR);
}

void Logger::Warning(std::string message)
{
    Logger::Log(message, Logger::WARNING);
}

void Logger::Info(std::string message)
{
    Logger::Log(message, Logger::INFO);
}

void Logger::Debug(std::string message)
{
    Logger::Log(message, Logger::DEBUG);
}

void Logger::Trace(std::string message)
{
    Logger::Log(message, Logger::TRACE);
}
