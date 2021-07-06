#ifndef LOG_IHANDLER_H
#define LOG_IHANDLER_H

#include "../../AllowedStd.hpp"
#include "../detail/Message.hpp"

namespace ft { namespace log
{
    typedef std::string(*FormatFunc)(const detail::Message &);

    class AHandler
    {
    protected:
        FormatFunc m_Format;

    public:
        explicit AHandler(FormatFunc func): m_Format(func) {}
        virtual ~AHandler() {}

        virtual void write(const detail::Message & msg) = 0;

    private:
        AHandler();
    };
} }

#endif //LOG_IHANDLER_H
