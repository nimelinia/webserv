#ifndef CONFIG_CONFIGEXCEPTION_H
#define CONFIG_CONFIGEXCEPTION_H

#include "../../AllowedStd.hpp"

namespace ft { namespace cfg
{
    class ConfigException: public std::runtime_error
    {
    public:
        explicit ConfigException(const std::string & what);
        ConfigException(const ConfigException & other) throw();
        virtual ~ConfigException() throw();
    };
} }

#endif //CONFIG_CONFIGEXCEPTION_H
