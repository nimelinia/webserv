#include "LoggerBase.hpp"
#include "Message.hpp"

ft::log::detail::LoggerBase::LoggerBase()
    : m_MaxLevel(ENone)
{
}

ft::log::detail::LoggerBase::~LoggerBase()
{
    for (std::list<AHandler*>::iterator it = m_HandlerList.begin(); it != m_HandlerList.end(); ++it)
        delete *it;
}

void ft::log::detail::LoggerBase::setMaxLevel(ft::log::ELevel lvl)
{
    m_MaxLevel = lvl;
}

void ft::log::detail::LoggerBase::addHandler(ft::log::AHandler * handler)
{
    if (handler)
        m_HandlerList.push_back(handler);
}

void ft::log::detail::LoggerBase::operator<<=(Message & msg)
{
    if (m_MaxLevel < msg.level())
        return;
    for (std::list<AHandler*>::iterator it = m_HandlerList.begin(); it != m_HandlerList.end(); ++it)
        (*it)->write(msg);
}
