#include "ConfigException.hpp"

ft::cfg::ConfigException::ConfigException(const std::string & what)
    : std::runtime_error(what)
{
}

ft::cfg::ConfigException::ConfigException(const ConfigException & other) throw()
    : std::runtime_error(other)
{
}

ft::cfg::ConfigException::~ConfigException() throw()
{
}

