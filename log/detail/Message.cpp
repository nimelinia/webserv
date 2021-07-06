#include "Message.hpp"
#include "../../util/String.hpp"

ft::log::detail::Message::Message(ft::log::ELevel lvl, const char* file, const char* func, size_t line)
    : m_Level(lvl), m_File(file), m_Func(func), m_Line(line)
{
    ::time(&m_Time);
}

ft::log::detail::Message & ft::log::detail::Message::ref()
{
    return *this;
}

ft::log::ELevel ft::log::detail::Message::level() const
{
    return m_Level;
}

const std::string & ft::log::detail::Message::text() const
{
    return m_Text;
}

const time_t & ft::log::detail::Message::time() const
{
    return m_Time;
}

const char* ft::log::detail::Message::file() const
{
    return m_File;
}

const char* ft::log::detail::Message::func() const
{
    return m_Func;
}

size_t ft::log::detail::Message::line() const
{
    return m_Line;
}

ft::log::detail::Message & ft::log::detail::Message::operator<<(const char* val)
{
    m_Text.append(val);
    return *this;
}

ft::log::detail::Message & ft::log::detail::Message::operator<<(const std::string & val)
{
    m_Text.append(val);
    return *this;
}

ft::log::detail::Message & ft::log::detail::Message::operator<<(bool val)
{
    m_Text.append(val ? "true" : "false");
    return *this;
}

ft::log::detail::Message & ft::log::detail::Message::operator<<(short val)
{
    m_Text.append(util::str::ToString(val));
    return *this;
}

ft::log::detail::Message & ft::log::detail::Message::operator<<(unsigned short val)
{
    m_Text.append(util::str::ToString(val));
    return *this;
}

ft::log::detail::Message & ft::log::detail::Message::operator<<(int val)
{
    m_Text.append(util::str::ToString(val));
    return *this;
}

ft::log::detail::Message & ft::log::detail::Message::operator<<(unsigned int val)
{
    m_Text.append(util::str::ToString(val));
    return *this;
}

ft::log::detail::Message & ft::log::detail::Message::operator<<(long val)
{
    m_Text.append(util::str::ToString(val));
    return *this;
}

ft::log::detail::Message & ft::log::detail::Message::operator<<(unsigned long val)
{
    m_Text.append(util::str::ToString(val));
    return *this;
}

