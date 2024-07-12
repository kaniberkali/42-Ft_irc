#ifndef LOGGER_HPP
# define LOGGER_HPP

#include <iostream>

class Logger
{
    private:
        static const std::string FATAL_COLOR;
        static const std::string ERROR_COLOR;
        static const std::string WARNING_COLOR;
        static const std::string INFO_COLOR;
        static const std::string DEBUG_COLOR;
        static const std::string TRACE_COLOR;
        static const std::string END_COLOR;
    public:
        static const int FATAL;
        static const int ERROR;
        static const int WARNING;
        static const int INFO;
        static const int DEBUG;
        static const int TRACE;
        static void Log(std::string message, int level);
        static void Fatal(std::string message);
        static void Error(std::string message);
        static void Warning(std::string message);
        static void Info(std::string message);
        static void Debug(std::string message);
        static void Trace(std::string message);
};

#endif
