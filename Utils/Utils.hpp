#ifndef UTILS_HPP
# define UTILS_HPP

#include <iostream>
#include <string>
#include <vector>

class Utils
{
    private:

    public:
        static std::string time();
        static std::string trim(const std::string &s);

        template <typename T>
        static std::string toString(const T& value);

        static std::vector<std::string> split(const std::string &s, char delim);
};


#endif
