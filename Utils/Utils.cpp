#include "Utils.hpp"
#include <sstream>

std::string Utils::time(std::string format)
{
    time_t now = ::time(0);
    tm *ltm = localtime(&now);

    for (size_t i = 0; i < format.length(); i++)
    {
        if (format[i] == 'Y')
        {
            format.replace(i, 1, Utils::toString(1900 + ltm->tm_year));
            i += 3;
        }
        else if (format[i] == 'm')
        {
            format.replace(i, 1, Utils::toString(1 + ltm->tm_mon));
            i += 1;
        }
        else if (format[i] == 'd')
        {
            format.replace(i, 1, Utils::toString(ltm->tm_mday));
            i += 1;
        }
        else if (format[i] == 'H')
        {
            format.replace(i, 1, Utils::toString(ltm->tm_hour));
            i += 1;
        }
        else if (format[i] == 'i')
        {
            format.replace(i, 1, Utils::toString(ltm->tm_min));
            i += 1;
        }
        else if (format[i] == 's')
        {
            format.replace(i, 1, Utils::toString(ltm->tm_sec));
            i += 1;
        }
    }
    return format;
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

std::vector<std::string> Utils::split(const std::string &s, const std::string &delim)
{
    std::vector<std::string> elems;
    size_t start = 0;
    size_t end = s.find(delim);
    while (end != std::string::npos)
    {
        elems.push_back(s.substr(start, end - start));
        start = end + delim.length();
        end = s.find(delim, start);
    }
    elems.push_back(s.substr(start));
    return elems;
}
