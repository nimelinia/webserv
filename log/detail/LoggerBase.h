#ifndef LOG_LOGGERBASE_H
#define LOG_LOGGERBASE_H

#include "AllowedStd.h"
#include "util/NonCopyable.h"
#include "log/Level.h"
#include "Message.h"
#include "log/handlers/AHandler.h"

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
