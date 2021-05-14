#include "String.h"

std::list<std::string> ft::util::str::Split(const std::string& str, char ch)
{
    std::list<std::string> split;
    const char* start = str.c_str();
    const char* cur = start;

    while (*cur != '\0')
    {
        if (*cur == ch)
        {
            split.push_back(std::string(start, cur - start));
            start = cur + 1;
        }
        cur++;
    }

    if (start != cur)
        split.push_back(std::string(start, cur - start));

    return split;
}

