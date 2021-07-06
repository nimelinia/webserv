#include "server/Select.hpp"
#include "CgiProcess.hpp"
#include "util/String.hpp"
#include "log/Log.hpp"


ft::http::CgiProcess::CgiProcess()
    : read_file(NULL)
    , bytes_written(0)
    , bytes_read(0)
    , m_pid(-1)
    , m_method_type(EUnknown)
    , m_state(EError)
{
}

ft::http::CgiProcess::EState ft::http::CgiProcess::state() const
{
    return m_state;
}

void ft::http::CgiProcess::clear()
{
    if (m_pid != -1 && (::waitpid(m_pid, NULL, WNOHANG) == 0))
    {
        ::kill(m_pid, SIGKILL);
        ::waitpid(m_pid, NULL, 0);
        m_pid = -1;
    }
    m_state = EIdle;
    if (read_file)
    {
        std::fclose(read_file);
        read_file = NULL;
    }
}

bool ft::http::CgiProcess::is_done()
{
	if (m_pid != -1 && ::waitpid(m_pid, NULL, WNOHANG) > 0)
	{
		m_pid = -1;
		return true;
	}
    return false;
}
