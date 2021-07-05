#include "PathException.hpp"
ft::cfg::PathException::PathException(const std::string& msg, const std::string& path)
    : ConfigException(Format(msg, path))
    , m_Message(msg), m_Path(path)
{
}

ft::cfg::PathException::PathException(const ft::cfg::PathException& other) throw()
    : ConfigException(other)
    , m_Message(other.m_Message), m_Path(other.m_Path)
{
}

ft::cfg::PathException::~PathException() throw()
{
}

const std::string& ft::cfg::PathException::message() const
{
    return m_Message;
}

const std::string& ft::cfg::PathException::path() const
{
    return m_Path;
}

std::string ft::cfg::PathException::Format(const std::string& msg, const std::string& path)
{
    std::string fmt;

    fmt += "(";
    fmt += path;
    fmt += "): ";
    fmt += msg;

    return msg;
}
