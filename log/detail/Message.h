#ifndef LOG_MESSAGE_H
#define LOG_MESSAGE_H

#include "../../AllowedStd.hpp"
#include "../Level.hpp"

namespace ft { namespace log { namespace detail
{
    class Message
    {
    private:
        ELevel m_Level;
        time_t m_Time;
        const char* m_File;
        const char* m_Func;
        size_t m_Line;
        std::string m_Text;

    public:
        Message(ELevel lvl, const char * file, const char * func, size_t line);

        Message & ref();

        ELevel level() const;
        const std::string & text() const;
        const time_t & time() const;
        const char* file() const;
        const char* func() const;
        size_t line() const;

        Message & operator<<(const char* val);
        Message & operator<<(const std::string & val);
        Message & operator<<(bool val);
        Message & operator<<(short val);
        Message & operator<<(unsigned short val);
        Message & operator<<(int val);
        Message & operator<<(unsigned int val);
        Message & operator<<(long val);
        Message & operator<<(unsigned long val);
    };
} } }

#endif //LOG_MESSAGE_H
