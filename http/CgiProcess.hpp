#ifndef HTTP_CGIPROCESS_HPP
#define HTTP_CGIPROCESS_HPP

#include "webserv.hpp"
#include "ResponseHandler.hpp"

namespace ft
{
    class Client;
}

namespace ft { namespace http
{
    class CgiProcess
    {
        friend class CgiHandler;
    public:
        enum EState
        {
            EError,
            EIdle,
            ESpawn,
            EWrite,
            ERead
        };

    private:
        enum EMethodType
        {
            EUnknown,
            EGet,
            EPost
        };

    private:
        pid_t m_pid;
        int m_cgi_fd[2];
        EMethodType m_method_type;
        EState      m_state;
        std::string m_body;

    public:
        CgiProcess();

        EState state() const;
        int max_fd() const;
        int read_fd() const;
        int write_fd() const;
        const std::string& body() const;
        void clear();
        bool update_state();
        bool read();
        bool write();
    };
} }

#endif //HTTP_CGIPROCESS_HPP
