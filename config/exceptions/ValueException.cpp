#include "ValueException.hpp"

ft::cfg::ValueException::ValueException(const std::string & what)
    :ConfigException(what)
{
}

ft::cfg::ValueException::ValueException(const ValueException & other) throw()
    :ConfigException(other)
{
}

ft::cfg::ValueException::~ValueException() throw()
{
}