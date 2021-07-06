#include "Formatters.hpp"
#include "Level.hpp"
#include "../util/String.hpp"
#include "detail/Message.hpp"

std::string ft::log::TextOnlyFormatter(const detail::Message & msg)
{
    std::string res;
    res += msg.text();
    return res;
}

std::string ft::log::TextWithLevelFormatter(const detail::Message & msg)
{
    std::string res;
    res += ft::log::LevelToString(msg.level());
    if (res.size() < 5)
        res.append(5 - res.size(), ' ');
    res += " | ";
    res += msg.text();
    return res;
}

std::string ft::log::TextWithTimeFormatter(const detail::Message & msg)
{
    char buf[21];
    struct tm * timeinfo = localtime(&msg.time());
    ::strftime(buf, 80, "%F %T ", timeinfo);

    std::string res;
    res += msg.text();
    return std::string(buf) + res;
}

std::string ft::log::SourceFormatter(const detail::Message & msg)
{
    std::string res;
    res += ft::log::LevelToString(msg.level());
    if (res.size() < 5)
        res.append(5 - res.size(), ' ');
    res += " [";
    res += msg.file();
    res += "@";
    res += msg.func();
    res += ":";
    res += ft::util::str::ToString(msg.line());
    res += "] ";
    res += msg.text();
    return res;
}

std::string ft::log::FullFormatter(const detail::Message & msg)
{
    char buf[21];
    struct tm * timeinfo = localtime(&msg.time());
    ::strftime(buf, 80, "%F %T ", timeinfo);

    std::string res;
    res += ft::log::LevelToString(msg.level());
    if (res.size() < 5)
        res.append(5 - res.size(), ' ');
    res += " [";
    res += msg.file();
    res += "@";
    res += msg.func();
    res += ":";
    res += ft::util::str::ToString(msg.line());
    res += "] ";
    res += msg.text();

    return std::string(buf) + res;
}
