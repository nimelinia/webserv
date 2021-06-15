#ifndef UTIL_STRING_H
#define UTIL_STRING_H

#include "../AllowedStd.h"

namespace ft { namespace util { namespace str
{
    template<class T>
    std::string ToString(T value);

    std::list<std::string> Split(const std::string & str, char ch);
} } }

template<class T>
std::string ft::util::str::ToString(T value)
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

#endif //UTIL_STRING_H
