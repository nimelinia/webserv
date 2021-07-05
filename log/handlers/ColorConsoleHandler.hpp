#ifndef LOG_COLORCONSOLEHANDLER_H
#define LOG_COLORCONSOLEHANDLER_H

#include "../../AllowedStd.hpp"
#include "AHandler.hpp"

namespace ft { namespace log
{
    class ColorConsoleHandler: public AHandler
    {
    public:
        explicit ColorConsoleHandler(FormatFunc func);

        virtual void write(const detail::Message & msg);

    private:
        static void SetColor(const detail::Message & msg);
        static void ResetColor(const detail::Message & msg);
    };
} }

#endif //LOG_COLORCONSOLEHANDLER_H
