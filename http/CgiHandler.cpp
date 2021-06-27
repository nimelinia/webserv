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

    if (pipe(process.m_cgi_fd) == -1)
    {
        LOGE_(CGI) << "pipe() failed";
        return process;
    }

    std::string env_str[] = {
        "SERVER_SOFTWARE=webserv",
        std::string("SERVER_SOFTWARE=") + m_config.hostaddress,
        "GATEWAY_INTERFACE=CGI/1.1",
        "SERVER_PROTOCOL=HTTP/1.1",
        std::string("SERVER_PORT=") + ft::util::str::ToString(m_config.port),
        std::string("REQUEST_METHOD=") + m_client.m_msg.m_method,
        "REDIRECT_STATUS=200",
        _env_path_info(),
        _env_path_translated(),
        _env_script_name(),
        _env_query_string(),
        _env_script_filename()
    };
    const size_t env_size = sizeof(env_str) / sizeof(std::string);
    std::string script_path= m_uri.root;
    script_path += m_uri.path;
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
        if (dup2(process.m_cgi_fd[0], STDIN_FILENO) == -1)
            exit(1);
        if (dup2(process.m_cgi_fd[1], STDOUT_FILENO) == -1)
            exit(1);
        close(process.m_cgi_fd[0]);
        close(process.m_cgi_fd[1]);
        const char** env = new const char*[env_size + 1];
        env[env_size] = NULL;

        env[0] = env_str[0].c_str();
        env[1] = env_str[1].c_str();
        env[2] = env_str[2].c_str();

        env[3] = env_str[3].c_str();
        env[4] = env_str[4].c_str();
        env[5] = env_str[5].c_str();
        env[6] = env_str[6].c_str();
        env[7] = env_str[7].c_str();
        env[8] = env_str[8].c_str();
        env[9] = env_str[9].c_str();
        env[10] = env_str[10].c_str();
        env[11] = env_str[11].c_str();

        execve(args[0], const_cast<char**>(args), const_cast<char**>(env));
        exit(1);
    }

    LOGI_(CGI) << "Cgi process spawned";

    Select::get().set_fd(process.m_cgi_fd[0]);
    Select::get().set_fd(process.m_cgi_fd[1]);

    if (m_client.m_msg.m_method == "GET")
        process.m_method_type = CgiProcess::EGet;
    else if (m_client.m_msg.m_method == "POST")
        process.m_method_type = CgiProcess::EPost;
    process.m_state = CgiProcess::ESpawn;
    return process;
}

std::string ft::http::CgiHandler::_env_path_info() const
{
    std::string str = "PATH_INFO=";
    str += m_uri.extra_path;
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
    str += m_uri.root;
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