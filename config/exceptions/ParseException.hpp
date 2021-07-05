#ifndef CONFIG_EXCEPTIONS_H
#define CONFIG_EXCEPTIONS_H

#include "ConfigException.hpp"

namespace ft { namespace cfg
{
    class ParseException: public ConfigException
    {
    private:
        std::string m_Message;
        unsigned long m_Line;

    public:
        ParseException(const std::string & msg, unsigned long line);
        ParseException(const ParseException & other) throw();
        virtual ~ParseException() throw();

        const std::string & message() const;
        unsigned long line() const;

        static std::string Format(const std::string & msg, unsigned long line);
    };
} }

#endif //CONFIG_EXCEPTIONS_H
