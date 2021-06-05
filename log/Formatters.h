#ifndef LOG_FORMATTERS_H
#define LOG_FORMATTERS_H

#include "AllowedStd.h"
#include "log/detail/Message.h"

namespace ft { namespace log
{
    std::string TextOnlyFormatter(const detail::Message & msg);
    std::string TextWithLevelFormatter(const detail::Message & msg);
    std::string SourceFormatter(const detail::Message & msg);
    std::string FullFormatter(const detail::Message & msg);
} }

#endif //LOG_FORMATTERS_H
