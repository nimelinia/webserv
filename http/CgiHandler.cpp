#include "Select.hpp"
#include "Client.hpp"
#include "CgiHandler.hpp"
#include "util/String.h"
#include "log/Log.h"

ft::http::CgiHandler::CgiHandler(const Config& cfg, Client& client, const Uri& uri)
    : m_config(cfg)
    , m_client(client)
    , m_uri(uri)
{
}

ft::http::CgiProcess ft::http::CgiHandler::spawn_cgi_process(const Locations& loc)
{
    Answer& answer = m_client.m_answer;
    CgiProcess process;

    if (m_client.m_msg.m_method == "GET")
        process.m_method_type = CgiProcess::EGet;
    else if (m_client.m_msg.m_method == "POST")
        process.m_method_type = CgiProcess::EPost;

    int read_fd[2];
    int write_fd[2];
    if (pipe(read_fd) == -1)
    {
        LOGE_(CGI) << "read pipe() failed";
        return process;
    }
    if (process.m_method_type == CgiProcess::EPost)
    {
        if (pipe(write_fd) == -1)
        {
            LOGE_(CGI) << "write pipe() failed";
            return process;
        }
    }

    std::vector<std::string> env_str;
    env_str.reserve(14);
    env_str.push_back("SERVER_SOFTWARE=webserv");
    env_str.push_back(std::string("SERVER_ADDRESS=") + m_config.hostaddress);
    env_str.push_back("GATEWAY_INTERFACE=CGI/1.1");
    env_str.push_back("SERVER_PROTOCOL=HTTP/1.1");
    env_str.push_back(std::string("SERVER_PORT=") + ft::util::str::ToString(m_config.port));
    env_str.push_back(std::string("REQUEST_METHOD=") + m_client.m_msg.m_method);
//    env_str.push_back(_env_path_info());
//    env_str.push_back(_env_path_translated());
//    env_str.push_back(_env_script_name());
//    env_str.push_back(_env_query_string());
    env_str.push_back(_env_script_filename());
    env_str.push_back("REDIRECT_STATUS=200");
    if (process.m_method_type == CgiProcess::EPost)
    {
        env_str.push_back("CONTENT_TYPE=application/x-www-form-urlencoded");
        env_str.push_back(std::string("CONTENT_LENGTH=") + util::str::ToString(m_client.m_msg.m_body.size()));
    }

    const size_t env_size = env_str.size();
    std::string script_path = m_uri.path;
    script_path += m_uri.file_name;

    const char* args[] = {
            loc.cgi.second.c_str(),
            script_path.c_str(),
            NULL
    };

    LOGD_(CGI) << "Cgi arguments: " << args[0] << " " << args[1];
    LOGD_(CGI) << "Cgi environment: ";
    for (size_t i = 0; i < env_size; ++i)
        LOGD_(CGI) << "\t" << env_str[i];

    process.m_pid = fork();
    if (process.m_pid == -1)
    {
        LOGE_(CGI) << "fork() failed";
        return process;
    }
    else if (process.m_pid == 0)
    {
        if (::chdir(m_uri.root.c_str()) == -1)
            ::exit(1);

        if (::dup2(read_fd[1], STDOUT_FILENO) == -1)
            ::exit(1);
        ::close(read_fd[0]);
        ::close(read_fd[1]);
        if (process.m_method_type == CgiProcess::EPost)
        {
            if (::dup2(write_fd[0], STDIN_FILENO) == -1)
                ::exit(1);
            ::close(write_fd[0]);
            ::close(write_fd[1]);
        }

        const char** env = new const char*[env_size + 1];
        env[env_size] = NULL;

        for (size_t i = 0; i < env_size; ++i)
            env[i] = env_str[i].c_str();

        ::execve(args[0], const_cast<char**>(args), const_cast<char**>(env));
        ::exit(1);
    }

    LOGI_(CGI) << "Cgi process spawned";

    ::close(read_fd[1]);
    Select::get().set_fd(read_fd[0]);
    process.read_fd = read_fd[0];
    if (process.m_method_type == CgiProcess::EPost)
    {
        ::close(write_fd[0]);
        Select::get().set_fd(write_fd[1]);
        process.write_fd = write_fd[1];
    }

    process.m_state = CgiProcess::ESpawn;
    return process;
}

void ft::http::CgiHandler::parse_cgi_body()
{
    Answer& answer = m_client.m_answer;
    const std::string& body = answer.m_body;
    std::string::size_type header_end = body.find("\r\n\r\n");
    if (header_end == std::string::npos)
        answer.m_status_code = 200;
    else
    {
        std::string::size_type line_start = 0;
        while (line_start < header_end)
        {
            std::string::size_type line_end = body.find("\r\n", line_start);
            std::string::size_type delim = body.find(": ", line_start);
            answer.m_headers.push_back((http::Header){
                    body.substr(line_start, delim - line_start),
                    body.substr(delim + 2, line_end - delim - 2)});
            line_start = line_end + 2;
        }
        answer.m_body = body.substr(header_end + 4);
        answer.m_status_code = 200;
    }
}

std::string ft::http::CgiHandler::_env_path_info() const
{
    std::string str = "PATH_INFO=";
    str += m_uri.path;
    str += m_uri.file_name;
    return str;
}

std::string ft::http::CgiHandler::_env_path_translated() const
{
    std::string str = "PATH_TRANSLATED=";
    if (!m_uri.extra_path.empty())
    {
        str += m_uri.root;
        str += m_uri.extra_path;
    }
    return str;
}

std::string ft::http::CgiHandler::_env_script_name() const
{
    std::string str = "SCRIPT_NAME=";
    str += m_uri.path;
    str += m_uri.file_name;
    return str;
}

std::string ft::http::CgiHandler::_env_script_filename() const
{
    std::string str = "SCRIPT_FILENAME=";
    str += m_uri.path;
    str += m_uri.file_name;
    return str;
}

std::string ft::http::CgiHandler::_env_query_string() const
{
    std::string str = "QUERY_STRING=";
    str += m_uri.query;
    return str;
}
