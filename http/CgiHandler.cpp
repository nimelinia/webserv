#include "Select.hpp"
#include "Client.hpp"
#include "CgiHandler.hpp"
#include "util/String.h"
#include "log/Log.h"
#include <stdio.h>

#define CR '\r'
#define LR '\n'

ft::http::CgiHandler::CgiHandler(const Config& cfg, Client& client, const Uri& uri)
    : m_config(cfg)
    , m_client(client)
    , m_uri(uri)
{
}

ft::http::CgiProcess ft::http::CgiHandler::spawn_cgi_process(const Locations& loc)
{
    Answer& answer = *m_client.m_answer;
    Message& msg = *m_client.m_msg;
    CgiProcess process;

    if (m_client.m_msg->m_method == "GET")
        process.m_method_type = CgiProcess::EGet;
    else if (m_client.m_msg->m_method == "POST")
        process.m_method_type = CgiProcess::EPost;

//    int read_fd[2];
    int read_fd;
    FILE* tmp_file;
    FILE* read_file;
    int write_fd;
//    int write_fd[2];
//    if (pipe(read_fd) == -1)
//    {
//        LOGE_(CGI) << "read pipe() failed";
//        return process;
//    }
    read_file = std::tmpfile();
    read_fd = ::fileno(read_file);
    tmp_file = std::tmpfile();
    write_fd = ::fileno(tmp_file);
    LOGD << "CGI | read fd: " << read_fd << " write fd: " << write_fd;
    if (process.m_method_type == CgiProcess::EPost)
    {
        std::fwrite(msg.m_body.c_str(), sizeof(char), msg.m_body.size(), tmp_file);
        std::rewind(tmp_file);
//        if (pipe(write_fd) == -1)
//        {
//            LOGE_(CGI) << "write pipe() failed";
//            return process;
//        }
    }

    m_path_info = loc.cgi.second;

    std::vector<std::string> env_arr;
    env_arr.reserve(14);
    env_arr.push_back("SERVER_SOFTWARE=webserv");
    env_arr.push_back(std::string("SERVER_ADDRESS=") + m_config.hostaddress);
    env_arr.push_back("GATEWAY_INTERFACE=CGI/1.1");
    env_arr.push_back("SERVER_PROTOCOL=HTTP/1.1");
    env_arr.push_back(std::string("SERVER_PORT=") + ft::util::str::ToString(m_config.port));
    env_arr.push_back(std::string("REQUEST_METHOD=") + m_client.m_msg->m_method);
    env_arr.push_back(_env_path_info());
//    env_arr.push_back(_env_path_translated());
//    env_arr.push_back(_env_script_name());
    env_arr.push_back(_env_query_string());
    env_arr.push_back(_env_script_filename());
    env_arr.push_back("REDIRECT_STATUS=200");

    for (std::vector<http::Header>::iterator it = msg.m_headers.begin(); it != msg.m_headers.end(); ++it)
    {
        std::string env_header;
        if (it->name != "content_length" && it->name != "content_type")
            env_header = "HTTP_";
        for (size_t i = 0; i < it->name.size(); ++i)
        {
            if (it->name[i] == '-')
                env_header.push_back('_');
            else
                env_header.push_back(static_cast<char>(std::toupper(it->name[i])));
        }
        env_header.push_back('=');
        env_header += it->value;

        env_arr.push_back(env_header);
    }

    const size_t env_size = env_arr.size();
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
        LOGD_(CGI) << "\t" << env_arr[i];

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
//        if (::dup2(read_fd[1], STDOUT_FILENO) == -1)
        if (::dup2(read_fd, STDOUT_FILENO) == -1)
            ::exit(1);
        ::close(read_fd);
//        ::close(read_fd[1]);
//        ::close(read_fd[1]);
        if (::dup2(write_fd, STDIN_FILENO) == -1)
            ::exit(1);
        ::close(write_fd);
//        if (process.m_method_type == CgiProcess::EPost)
//        {
////            if (::dup2(write_fd[0], STDIN_FILENO) == -1)
//            if (::dup2(write_fd, STDIN_FILENO) == -1)
//                ::exit(1);
//            ::close(write_fd);
////            ::close(write_fd[0]);
////            ::close(write_fd[1]);
//        }
//        else
//            ::close(STDIN_FILENO);

        const char** env = new const char*[env_size + 1];
        env[env_size] = NULL;

        for (size_t i = 0; i < env_size; ++i)
            env[i] = env_arr[i].c_str();

        ::execve(args[0], const_cast<char**>(args), const_cast<char**>(env));
        ::exit(1);
    }

//    LOGI_(CGI) << "Cgi process spawned";

//    ::close(read_fd[1]);
//    Select::get().set_fd(read_fd[0]);
//    process.read_fd = read_fd[0];
    process.read_file = read_file;
    std::fclose(tmp_file);
//    if (process.m_method_type == CgiProcess::EPost)
//    {
//        std::fclose(tmp_file);
////        ::close(write_fd[0]);
////        Select::get().set_fd(write_fd[1]);
////        process.write_fd = write_fd[1];
//    }

    process.m_state = CgiProcess::ESpawn;
    return process;
}

bool ft::http::CgiHandler::parse_cgi_body()
{
    Answer& answer = *m_client.m_answer;

    char c;
    EParseState state = EHeaderStart;
    std::rewind(m_client.m_cgi_process.read_file);
    while (std::fread(&c, 1, 1, m_client.m_cgi_process.read_file) > 0)
    {
        switch (state)
        {
            case EHeaderStart:
                if (c == CR)
                {
                    if (answer.m_headers.empty())
                        return false;
                    state = ENewLine2;
                    continue;
                }
                else if (!std::isalpha(c))
                    return false;
                else
                {
                    answer.m_headers.push_back(Header());
                    answer.m_headers.back().name.push_back(static_cast<char>(std::tolower(c)));
                    state = EHeaderName;
                    continue;
                }
            case EHeaderName:
                if (c == ':')
                {
                    state = EHeaderSpace;
                    continue;
                }
                else if (std::isalpha(c) || c == '-')
                {
                    answer.m_headers.back().name.push_back(static_cast<char>(std::tolower(c)));
                    continue;
                }
                else
                    return false;
            case EHeaderSpace:
                if (c == ' ')
                {
                    state = EHeaderValue;
                    continue;
                }
                else
                    return false;
            case EHeaderValue:
                if (c == CR)
                {
                    state = ENewLine1;
                    continue;
                }
                else if (std::iscntrl(c))
                    return false;
                else
                {
                    answer.m_headers.back().value.push_back(static_cast<char>(std::tolower(c)));
                    continue;
                }
            case ENewLine1:
                if (c == LR)
                {
                    state = EHeaderStart;
                    continue;
                }
                else
                    return false;
            case ENewLine2:
                if (c == LR)
                {
                    _parse_headers();
                    return _set_content_length();
                }
                else
                    return false;
            default:
                return false;
        }
    }
    return false;
}

std::string ft::http::CgiHandler::_env_path_info() const
{
    std::string str = "PATH_INFO=";
    str += m_path_info;
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

void ft::http::CgiHandler::_parse_headers()
{
    Answer& answer = *m_client.m_answer;
    std::list<http::Header>::iterator it = std::find_if(answer.m_headers.begin(), answer.m_headers.end(),
            http::FindHeader("status"));

    if (it != answer.m_headers.end())
    {
        answer.m_status_code = std::strtoul(it->value.c_str(), NULL, 10);
        if (answer.m_status_code == 0)
            answer.m_status_code = 500;
        answer.m_headers.erase(it);
    }
}
bool ft::http::CgiHandler::_set_content_length()
{
    Answer& answer = *m_client.m_answer;
    CgiProcess& process = m_client.m_cgi_process;

    long cur_pos = std::ftell(process.read_file);
    if (std::fseek(process.read_file, 0, SEEK_END) == -1)
        return false;
    else
    {
        process.file_size = std::ftell(process.read_file) - cur_pos;
        if (std::fseek(process.read_file, cur_pos, SEEK_SET) == -1)
            return false;
        answer.m_headers.push_back((http::Header) {"Content-Length",
                                                     util::str::ToString(process.file_size)});
        return true;
    }
}
