#ifndef UTILS_HPP
# define UTILS_HPP

#include <iostream>
class Utils
{
    private:

    public:
        static std::string time();
        template <typename T>
        static std::string toString(const T& value);
};


#endif
