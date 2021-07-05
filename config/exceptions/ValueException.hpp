#ifndef CONFIG_VALUEEXCEPTION_H
#define CONFIG_VALUEEXCEPTION_H

#include "ConfigException.hpp"

namespace ft { namespace cfg
{
    class ValueException: public ConfigException
    {
    public:
        explicit ValueException(const std::string & what);
        ValueException(const ValueException & other) throw();
        virtual ~ValueException() throw();
    };
} }

#endif //CONFIG_VALUEEXCEPTION_H
