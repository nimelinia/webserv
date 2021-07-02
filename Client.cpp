//
// Created by Streetmentioner Copycat on 5/8/21.
//

#include "Client.hpp"
#include "Server.hpp"
#include "http/RequestParser.h"
#include "ResponseHandler.hpp"
#include "log/Log.h"

ft::Client::Client(int socketCl, Server *server) :
	m_state(e_request_parse),
	m_socket_cl(socketCl),
	m_server(server),
	m_answer(),		// тут нужно поменять, т.к. этот лимит лежит внутри location и мы сначала должны распарсить uri, а потом уже определять размер
	m_parsed(),
	m_buff(NULL)
{
}

ft::Client::~Client()
{
    delete[] m_buff;
}

bool ft::Client::read_message()
{
	ssize_t	ret;

	ret = recv(m_socket_cl, m_buff, BUFFER_SIZE, 0);
	if (ret == 0 || ret == -1)
		return (true);
	if (m_state == e_request_parse)
	{
        http::RequestParser::EResult res = m_parser.parse(m_server->m_configs, m_msg, m_buff, ret);
        if (res == http::RequestParser::EParse)
			return false;
		if (res == http::RequestParser::EOk)
		{
			LOGD << "URI: " << m_msg.m_uri_str;
			LOGD << "Method: " << m_msg.m_method;
			LOGD << "Headers:";
			for (std::vector<http::Header>::iterator hit = m_msg.m_headers.begin(); hit != m_msg.m_headers.end(); ++hit)
				LOGD << "\t" << hit->name << ": " << hit->value;
			LOGD;
			m_state = e_request_ready;
		}
		else if (res == http::RequestParser::EError)
		{
            LOGE << "Error parsing header";
			m_answer.m_status_code = m_msg.m_error_num;
			m_state = e_request_ready;
		}
	}
	if (m_state == e_request_ready)
	{
		ResponseHandler handler(*m_msg.m_uri.config, *this);
		bool result = true;
		if (!m_answer.m_status_code)
			result = handler.generate_answer();
		if (result)
		{
			handler.generate_status_body();
			m_state = e_response_ready;
		}

	}
	return (false);
}

bool ft::Client::send_message()
{
    if (m_state == e_response_ready) {
		if (m_msg.m_method == "HEAD")
			m_answer.m_body_exist = false;
		else
			m_answer.m_body_exist = true;
		m_answer.m_headers.push_back((http::Header) {"Content-Length",
													 util::str::ToString(m_answer.m_body.size())});
		m_answer.m_server = m_msg.m_uri.config->server_name;
		m_answer.create_final_response();
		m_state = e_sending;
	}
	ssize_t	ret;
	ret = send(m_socket_cl, m_answer.m_final_response.c_str(),  m_answer.m_final_response.size(), 0);
	if (ret == 0 || ret == -1) {
		return (true);
	}
	m_answer.m_final_response.erase(0, ret);
	if (m_answer.m_final_response.empty())																				// если не доотправлено, то в следующий раз по флажку пойдет отправлять
	{
		m_msg.m_ready_responce = false;
		m_parser.reset();
		m_msg.clean();
		m_answer.clean();
		m_cgi_process.clear();
		m_state = e_request_parse;
	}
	return (false);
}


void ft::Client::close()
{
	::close(m_socket_cl);																						// закрываю сокет
	Select::get().clear_fd(m_socket_cl);
}

bool ft::Client::ready_read() const
{
	return (m_state == e_request_parse);
}

bool ft::Client::ready_write() const
{
	return (m_state == e_response_ready || m_state == e_sending);
}

int ft::Client::max_fd() const
{
    return std::max(m_socket_cl, m_cgi_process.max_fd());
}

bool ft::Client::cgi_spawned()
{
    if (m_cgi_process.state() == http::CgiProcess::ESpawn)
    {
        if (m_cgi_process.update_state())
            return true;
        m_answer.m_status_code = 500;
        m_state = e_response_ready;
    }
    return false;
}

void ft::Client::init_buffer()
{
    m_buff = new char[BUFFER_SIZE];
}
void ft::Client::check_cgi()
{
    if (m_cgi_process.is_done())
    {
        m_state = e_response_ready;
        LOGI_(CGI) << "Cgi process finished";
    }
}

bool ft::Client::send_cgi_message()
{
    if (m_state == e_response_ready)
    {
        http::CgiHandler handler(*m_msg.m_uri.config, *this, Uri());
        if (!handler.parse_cgi_body())
            m_answer.m_status_code = 500;
        else
        {
            long cur_pos = std::ftell(m_cgi_process.read_file);
            if (std::fseek(m_cgi_process.read_file, 0, SEEK_END) == -1)
                m_answer.m_status_code = 500;
            else
            {
                m_cgi_process.file_size = std::ftell(m_cgi_process.read_file) - cur_pos;
                if (std::fseek(m_cgi_process.read_file, cur_pos, SEEK_SET) == -1)
                    m_answer.m_status_code = 500;
                m_answer.m_headers.push_back((http::Header) {"Content-Length",
                                                             util::str::ToString(m_cgi_process.file_size)});
            }
        }
        if (m_answer.m_status_code == 0)
            m_answer.m_status_code = 200;
        m_answer.create_final_response();
        m_state = e_sending;
    }
    ssize_t	ret;
    if (!m_answer.m_final_response.empty())
        ret = send(m_socket_cl, m_answer.m_final_response.c_str(),  m_answer.m_final_response.size(), 0);
    else
    {
        if (m_cgi_process.bytes_written == 0)
        {
            m_cgi_process.bytes_read = std::fread(m_buff, 1, BUFFER_SIZE, m_cgi_process.read_file);
            if (std::ferror(m_cgi_process.read_file))
                return true;
        }
        ret = ::send(m_socket_cl, m_buff + m_cgi_process.bytes_written,  m_cgi_process.bytes_read - m_cgi_process.bytes_written, 0);
        if (ret > 0)
            m_cgi_process.bytes_written += ret;
        if (m_cgi_process.bytes_written == m_cgi_process.bytes_read)
            m_cgi_process.bytes_written = 0;
    }
    if (ret == -1) {
        return (true);
    }
    else if (ret == 0)
        m_cgi_process.clear();
    if (!m_answer.m_final_response.empty())
        m_answer.m_final_response.erase(0, ret);
    if (m_cgi_process.state() == http::CgiProcess::EIdle)																				// если не доотправлено, то в следующий раз по флажку пойдет отправлять
    {
        m_msg.m_ready_responce = false;
        m_parser.reset();
        m_msg.clean();
        m_answer.clean();
        m_cgi_process.clear();
        m_state = e_request_parse;
    }
    return false;
}
