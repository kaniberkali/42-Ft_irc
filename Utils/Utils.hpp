#ifndef UTILS_HPP
# define UTILS_HPP

#include <iostream>
#include <string>
#include <vector>

class Utils
{
    private:

    public:
        static std::string date(std::string format = "Y-m-d H:i:s");
        static std::string getMonthName(int month);
        static std::string getDayName(int day);
        static std::string trim(const std::string &s);
        static std::string padStart(std::string str, size_t length = 2, char padChar = '0');
        static std::string toUpper(std::string str);
        static std::string toLower(std::string str);

        template <typename T>
        static std::string toString(const T& value);

        static int toInt(std::string value);

        static std::vector<std::string> split(const std::string &s, const std::string &delim);
};


#endif
