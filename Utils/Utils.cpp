#include "Utils.hpp"
#include <sstream>

std::string Utils::time()
{
    time_t now = ::time(0);
    tm *ltm = localtime(&now);
    std::string time = Utils::toString(1900 + ltm->tm_year);
    time += "-" + Utils::toString(1 + ltm->tm_mon);
    time += "-" + Utils::toString(ltm->tm_mday);
    time += " " + Utils::toString(ltm->tm_hour);
    time += ":" + Utils::toString(ltm->tm_min);
    time += ":" + Utils::toString(ltm->tm_sec);
    return time;
}

template <typename T>
std::string Utils::toString(const T& value)
{
    std::stringstream stringStream;
    stringStream << value;
    return stringStream.str();
}