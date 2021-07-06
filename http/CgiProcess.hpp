#ifndef HTTP_CGIPROCESS_HPP
#define HTTP_CGIPROCESS_HPP

#include "server/webserv.hpp"
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

    public:
        FILE* read_file;
        long file_size;
        size_t bytes_written;
        size_t bytes_read;

    private:
        pid_t m_pid;
        EMethodType m_method_type;
        EState      m_state;

    public:
        CgiProcess();

        EState state() const;
        void clear();
        bool update_state();
        bool is_done();
    };
} }

#endif //HTTP_CGIPROCESS_HPP
