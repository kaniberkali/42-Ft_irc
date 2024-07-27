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

std::string Utils::trim(const std::string &s)
{
    std::string::const_iterator start = s.begin();
    std::string::const_iterator end = s.end();

    while (start != end && std::isspace(*start))
        start++;

    if (start == end)
        return "";
    do
    {
        end--;
    } while (std::distance(start, end) > 0 && std::isspace(*end));

    return std::string(start, end + 1);
}

std::vector<std::string> Utils::split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim))
        elems.push_back(item);
    return elems;
}