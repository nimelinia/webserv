#include "Select.hpp"
#include "CgiProcess.hpp"
#include "util/String.h"
#include "log/Log.h"

#define BUFFER_SIZE     1024

ft::http::CgiProcess::CgiProcess()
    : m_pid(-1)
    , m_method_type(EUnknown)
    , m_state(EError)
{
    m_cgi_fd[0] = -1;
    m_cgi_fd[1] = -1;
}

ft::http::CgiProcess::EState ft::http::CgiProcess::state() const
{
    return m_state;
}

int ft::http::CgiProcess::max_fd() const
{
    return std::max(m_cgi_fd[0], m_cgi_fd[1]);
}

void ft::http::CgiProcess::clear()
{
    if (m_pid != -1 && (::waitpid(m_pid, NULL, WNOHANG) == 0))
    {
        ::kill(m_pid, SIGKILL);
        ::waitpid(m_pid, NULL, 0);
        m_pid = -1;
    }
    if (m_cgi_fd[0] != - 1)
    {
        Select::get().clear_fd(m_cgi_fd[0]);
        ::close(m_cgi_fd[0]);
        m_cgi_fd[0] = -1;
    }
    if (m_cgi_fd[1] != - 1)
    {
        Select::get().clear_fd(m_cgi_fd[1]);
        ::close(m_cgi_fd[1]);
        m_cgi_fd[1] = -1;
    }
}

int ft::http::CgiProcess::write_fd() const
{
    return m_cgi_fd[1];
}

int ft::http::CgiProcess::read_fd() const
{
    return m_cgi_fd[0];
}

void ft::http::CgiProcess::end_read(size_t ret)
{
    m_cgi_fd[0] = -1;
    if (ret == -1)
    {
        LOGD_(CGI) << "read() error: " << strerror(errno);
        m_state = EError;
    }
    else
        m_state = EIdle;
}

bool ft::http::CgiProcess::write()
{
    return true;
}

bool ft::http::CgiProcess::update_state()
{
    switch (m_method_type)
    {
        case EGet:
            ::close(m_cgi_fd[1]);
            Select::get().clear_fd(m_cgi_fd[1]);
            m_cgi_fd[1] = -1;
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

const std::string& ft::http::CgiProcess::body() const
{
    return m_body;
}
