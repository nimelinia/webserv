#ifndef UTIL_STRING_H
#define UTIL_STRING_H

#include "../AllowedStd.hpp"

namespace ft { namespace util { namespace str
{
    template<class T>
    std::string ToString(T value);

    template<class T>
    std::pair<T, bool> FromString(const std::string& str);

    std::list<std::string> Split(const std::string & str, char ch);

    template<class T>
    T DecodeHex(char c);
} namespace date
		{
			std::string get_date();
			std::string get_date(struct timespec time);
		}
}
}

template<class T>
std::string ft::util::str::ToString(T value)
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

template<class T>
std::pair<T, bool> ft::util::str::FromString(const std::string& str)
{
    T value;
    std::istringstream iss(str);
    iss >> value;
    return std::make_pair(value, iss.eof());
}

template<class T>
T ft::util::str::DecodeHex(char c)
{
    if ('A' <= c && c <= 'F')
        return c - 'A' + 0xA;
    else if ('a' <= c && c <= 'f')
        return c - 'a' + 0xA;
    else
        return c - '0';
}

#endif //UTIL_STRING_H
