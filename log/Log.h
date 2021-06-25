#ifndef LOG_LOG_H
#define LOG_LOG_H

#include "detail/LoggerBase.h"
#include "../util/Singleton.h"

#include "Formatters.h"

#include "handlers/ConsoleHandler.h"
#include "handlers/ColorConsoleHandler.h"
#include "handlers/FileHandler.h"

namespace ft { namespace log
{
    class Logger: public ft::log::detail::LoggerBase, public util::Singleton<Logger>
    {
    };
} }

#define LOGGER ft::log::Logger::get()
#define LOGGER_(id) ft::log::Logger::get<id>()

#define LOG_(id, level)  ft::log::Logger::get<id>() <<= ft::log::detail::Message(level, __FILE_NAME__, __PRETTY_FUNCTION__, __LINE__).ref()
#define LOG(level)  ft::log::Logger::get() <<= ft::log::detail::Message(level, __FILE_NAME__, __PRETTY_FUNCTION__, __LINE__).ref()

#define LOGF_(id)    LOG_(id, ft::log::EFatal)
#define LOGE_(id)    LOG_(id, ft::log::EError)
#define LOGW_(id)    LOG_(id, ft::log::EWarning)
#define LOGI_(id)    LOG_(id, ft::log::EInfo)
#define LOGD_(id)    LOG_(id, ft::log::EDebug)

#define LOGF    LOG(ft::log::EFatal)
#define LOGE    LOG(ft::log::EError)
#define LOGW    LOG(ft::log::EWarning)
#define LOGI    LOG(ft::log::EInfo)
#define LOGD    LOG(ft::log::EDebug)

#define CFG 0

#endif //LOG_LOG_H
