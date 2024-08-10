#include "Utils.hpp"
#include <sstream>

std::string Utils::date(std::string format) {
    time_t now = ::time(0);
    tm *ltm = localtime(&now);

    for (size_t i = 0; i < format.length(); i++) {
        if (format[i] == 'Y')
            format.replace(i, 1, Utils::toString(1900 + ltm->tm_year));
        else if (format[i] == 'm')
            format.replace(i, 1, padStart(Utils::toString(1 + ltm->tm_mon)));
        else if (format[i] == 'd')
            format.replace(i, 1, padStart(Utils::toString(ltm->tm_mday)));
        else if (format[i] == 'H')
            format.replace(i, 1, padStart(Utils::toString(ltm->tm_hour)));
        else if (format[i] == 'i')
            format.replace(i, 1, padStart(Utils::toString(ltm->tm_min)));
        else if (format[i] == 's')
            format.replace(i, 1, padStart(Utils::toString(ltm->tm_sec)));
        else if (format[i] == 'M')
            format.replace(i, 1, Utils::getMonthName(1 + ltm->tm_mon));
        else if (format[i] == 'z')
            format.replace(i, 1, "UTC");
        else if (format[i] == 'D')
            format.replace(i, 1, Utils::getDayName(ltm->tm_wday));
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

int Utils::toInt(std::string value)
{
    std::stringstream stringStream;
    stringStream << value;
    int intValue;
    stringStream >> intValue;
    return intValue;
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
    if (!s.substr(start).empty())
        elems.push_back(s.substr(start));
    return elems;
}

std::string Utils::getMonthName(int month) {
    static const char* monthNames[] = {
            "Jan", "Feb", "Mar", "Apr", "May", "Jun",
            "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };
    return monthNames[month - 1];
}

std::string Utils::getDayName(int day)
{
    static const char* dayNames[] = {
            "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
    };
    return dayNames[day];
}

std::string Utils::padStart(std::string str, size_t length, char padChar)
{
    if (str.length() >= length)
        return str;
    return std::string(length - str.length(), padChar) + str;
}

std::string Utils::toUpper(std::string str)
{
    for (size_t i = 0; i < str.length(); i++)
        str[i] = std::toupper(str[i]);
    return str;
}

std::string Utils::toLower(std::string str)
{
    for (size_t i = 0; i < str.length(); i++)
        str[i] = std::tolower(str[i]);
    return str;
}