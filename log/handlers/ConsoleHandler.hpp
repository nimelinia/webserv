#ifndef LOG_CONSOLEHANDLER_H
#define LOG_CONSOLEHANDLER_H

#include "../../AllowedStd.hpp"
#include "AHandler.hpp"

namespace ft { namespace log
{
    class ConsoleHandler: public AHandler
    {
    public:
        explicit ConsoleHandler(FormatFunc func);

        virtual void write(const detail::Message & msg);
    };
} }

#endif //LOG_CONSOLEHANDLER_H
