#ifndef HTTP_CGIHANDLER_HPP
#define HTTP_CGIHANDLER_HPP

#include "webserv.hpp"
#include "ResponseHandler.hpp"
#include "CgiProcess.hpp"

namespace ft
{
    class Client;
}

namespace ft { namespace http
{
    class CgiHandler
    {
    private:
        enum EParseState
        {
            EHeaderStart,
            EHeaderName,
            EHeaderSpace,
            EHeaderValue,
            ENewLine1,
            ENewLine2,
        };

    private:
        Client&             m_client;
        const Config&       m_config;
        const Uri&          m_uri;
        std::string         m_path_info;

    public:
        CgiHandler(const Config& cfg, Client& client, const Uri& m_uri);

        ft::http::CgiProcess spawn_cgi_process(const Locations& loc);
        bool parse_cgi_body();

    private:
        std::string _env_path_info() const;
        std::string _env_path_translated() const;
        std::string _env_script_name() const;
        std::string _env_script_filename() const;
        std::string _env_query_string() const;
        void _parse_headers();
    };
} }

#endif //HTTP_CGIHANDLER_HPP
