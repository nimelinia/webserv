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

    public:
        int read_fd;
        int write_fd;
        FILE* read_file;
        long file_size;
        ssize_t bytes_written;
        size_t bytes_read;

    private:
        pid_t m_pid;
        EMethodType m_method_type;
        EState      m_state;

    public:
        CgiProcess();

        EState state() const;
        int max_fd() const;
        void clear();
        bool update_state();
        void end_read(size_t ret);
        void end_write(size_t ret);
        bool is_done() const;
    };
} }

#endif //HTTP_CGIPROCESS_HPP
