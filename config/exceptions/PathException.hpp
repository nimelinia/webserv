#ifndef CONFIG_PATHEXCEPTION_H
#define CONFIG_PATHEXCEPTION_H

#include "ConfigException.hpp"

namespace ft { namespace cfg
{
    class PathException: public ConfigException
    {
    private:
        std::string m_Message;
        std::string m_Path;

    public:
        PathException(const std::string & msg, const std::string & path);
        PathException(const PathException & other) throw();
        virtual ~PathException() throw();

        const std::string & message() const;
        const std::string & path() const;

        static std::string Format(const std::string & msg, const std::string & path);
    };
} }

#endif //CONFIG_PATHEXCEPTION_H
