#ifndef LOGGER_HPP
# define LOGGER_HPP

#include <iostream>

class Logger
{
    public:
        enum Level
        {
            FATAL = 0,
            ERROR = 1,
            WARNING = 2,
            INFO = 3,
            DEBUG = 4,
            TRACE = 5,
            HEXCHAT = 6
        };
        static void Log(std::string message, int level,  int row = 0, int column = 0);
        static void Fatal(std::string message, int row = 0, int column = 0);
        static void Error(std::string message, int row = 0, int column = 0);
        static void Warning(std::string message, int row = 0, int column = 0);
        static void Info(std::string message, int row = 0, int column = 0);
        static void Debug(std::string message, int row = 0, int column = 0);
        static void Trace(std::string message, int row = 0, int column = 0);
        static void HexChat(std::string message, int row = 0, int column = 0);
};

#endif
