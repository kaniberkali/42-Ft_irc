#include "Logger.hpp"
#include "../Utils/Utils.hpp"

void Logger::Log(std::string message, int level, int row, int column)
{
    std::string print = "";
    if (level == Logger::FATAL)
        print += "\033[1;31m" + Utils::date() + " [FATAL] ";
    else if (level == Logger::ERROR)
        print += "\033[0;31m" + Utils::date() + " [ERROR] ";
    else if (level == Logger::WARNING)
        print += "\033[0;33m" + Utils::date() + " [WARNING] ";
    else if (level == Logger::INFO)
        print += "\033[0;34m" + Utils::date() + " [INFO] ";
    else if (level == Logger::DEBUG)
        print += "\033[0;32m" + Utils::date() + " [DEBUG] ";
    else if (level == Logger::TRACE)
        print += "\033[0;37m" + Utils::date() + " [TRACE] ";
    else if (level == Logger::HEXCHAT)
        print += "\033[1;36m" + Utils::date() + " [HEXCHAT] ";

    print += message;
    if (row != 0)
    {
        std::cout << "\033[0;0H" << "\033[999B" << "\033[" + Utils::toString(row) + "A";
        std::cout << "\033[2K";
        std::cout << "\033[" + Utils::toString(column) + "G";
        std::cout << print << "\033[0m" << std::flush;
    }
    else
        std::cout << print << "\033[0m" << std::flush;
    std::cout << std::endl;
}

void Logger::Fatal(std::string message, int row, int column)
{
    Logger::Log(message, Logger::FATAL, row, column);
}

void Logger::Error(std::string message, int row, int column)
{
    Logger::Log(message, Logger::ERROR, row, column);
}

void Logger::Warning(std::string message, int row, int column)
{
    Logger::Log(message, Logger::WARNING, row, column);
}

void Logger::Info(std::string message, int row, int column)
{
    Logger::Log(message, Logger::INFO, row, column);
}

void Logger::Debug(std::string message, int row, int column)
{
    Logger::Log(message, Logger::DEBUG, row, column);
}

void Logger::Trace(std::string message, int row, int column)
{
    Logger::Log(message, Logger::TRACE, row, column);
}

void Logger::HexChat(std::string message, int row, int column)
{
    Logger::Log(message, Logger::HEXCHAT, row, column);
}