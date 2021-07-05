#include "ParseException.hpp"
#include "../../util/String.hpp"

ft::cfg::ParseException::ParseException(const std::string & msg, unsigned long line)
    : ConfigException(ParseException::Format(msg, line))
    , m_Message(msg), m_Line(line)
{
}

ft::cfg::ParseException::ParseException(const ft::cfg::ParseException & other) throw()
    : ConfigException(other)
    , m_Message(other.m_Message), m_Line(other.m_Line)
{
}

ft::cfg::ParseException::~ParseException() throw()
{
}

const std::string & ft::cfg::ParseException::message() const
{
    return m_Message;
}

unsigned long ft::cfg::ParseException::line() const
{
    return m_Line;
}

std::string ft::cfg::ParseException::Format(const std::string & msg, unsigned long line)
{
    std::string fmt;

    if (line != 0)
    {
        fmt += "(Line ";
        fmt += util::str::ToString(line);
        fmt += "): ";
    }
    fmt += msg;

    return fmt;
}
