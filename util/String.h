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
    const bool isNeg = value < 0;
    unsigned long long modVal = isNeg ? -value : value;
    std::string str;

    do
    {
        const char ch = static_cast<char>(modVal % 10) + '0';
        str += ch;
        modVal /= 10;
    } while (modVal != 0);

    if (isNeg)
        str += "-";

    std::reverse(str.begin(), str.end());
    return str;
}

#endif //UTIL_STRING_H
