#include "Select.hpp"
#include "CgiProcess.hpp"
#include "util/String.h"
#include "log/Log.h"

#define BUFFER_SIZE     1024

ft::http::CgiProcess::CgiProcess()
    :m_pid(-1)
    , read_fd(-1), write_fd(-1)
    , m_method_type(EUnknown)
    , m_state(EError)
{
}

ft::http::CgiProcess::EState ft::http::CgiProcess::state() const
{
    return m_state;
}

int ft::http::CgiProcess::max_fd() const
{
    return std::max(read_fd, write_fd);
}

void ft::http::CgiProcess::clear()
{
    if (m_pid != -1 && (::waitpid(m_pid, NULL, WNOHANG) == 0))
    {
        ::kill(m_pid, SIGKILL);
        ::waitpid(m_pid, NULL, 0);
        m_pid = -1;
    }
    if (read_fd != - 1)
    {
        Select::get().clear_fd(read_fd);
        ::close(read_fd);
        read_fd = -1;
    }
    if (write_fd != - 1)
    {
        Select::get().clear_fd(write_fd);
        ::close(write_fd);
        write_fd = -1;
    }
}

void ft::http::CgiProcess::end_read(size_t ret)
{
    ::close(read_fd);
    Select::get().clear_fd(read_fd);
    read_fd = -1;
    if (ret == -1)
    {
        LOGD_(CGI) << "read() error: " << strerror(errno);
        m_state = EError;
    }
    else
        m_state = EIdle;
}

void ft::http::CgiProcess::end_write(size_t ret)
{
    ::close(write_fd);
    Select::get().clear_fd(write_fd);
    write_fd = -1;
    if (ret == -1)
    {
        LOGD_(CGI) << "write() error: " << strerror(errno);
        m_state = EError;
    }
    else
        m_state = ERead;
}

bool ft::http::CgiProcess::update_state()
{
    switch (m_method_type)
    {
        case EGet:
            m_state = ERead;
            return true;
        case EPost:
            m_state = EWrite;
            return true;
        case EUnknown:
            m_state = EError;
    }
    return false;
}