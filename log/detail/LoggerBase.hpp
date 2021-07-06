#ifndef LOG_LOGGERBASE_H
#define LOG_LOGGERBASE_H

#include "../../AllowedStd.hpp"
#include "../../util/NonCopyable.hpp"
#include "../Level.hpp"
#include "Message.hpp"
#include "../handlers/AHandler.hpp"

namespace ft { namespace log { namespace detail
{
    class LoggerBase: virtual util::NonCopyable
    {
    private:
        ELevel m_MaxLevel;
        std::list<AHandler*> m_HandlerList;

    public:
        LoggerBase();
        ~LoggerBase();

        void setMaxLevel(ELevel lvl);
        void addHandler(AHandler * handler);

        void operator<<=(Message & msg);
    };
} } }

#endif //LOG_LOGGERBASE_H
