//
// Created by Streetmentioner Copycat on 5/8/21.
//

#include "Client.hpp"
#include "Server.hpp"
#include "http/RequestParser.h"
#include "ResponseHandler.hpp"
#include "log/Log.h"
//#include "webserv.hpp"

ft::Client::Client(int socketCl, Server *server) :
	m_state(e_request_parse),
	m_socket_cl(socketCl),
	m_server(server),
	m_answer(),
	m_parsed()
{
	m_buff = new char[BUFFER_SIZE];
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

bool ft::Client::cgi_ready_read() const
{
    return m_cgi_process.state() == http::CgiProcess::ERead;
}

bool ft::Client::cgi_ready_write() const
{
    return m_cgi_process.state() == http::CgiProcess::EWrite;
}

bool ft::Client::cgi_read()
{
    if (m_cgi_process.read())
    {
        if (m_cgi_process.state() == http::CgiProcess::EError)
            m_answer.m_status_code = 500;
        else
        {
            const std::string& body = m_cgi_process.body();
            std::string::size_type header_end = body.find("\r\n\r\n");
            if (header_end == std::string::npos)
                m_answer.m_status_code = 200;
            else
            {
                m_answer.m_body = body.substr(header_end + 4);
                std::string::size_type line_start = 0;
                while (line_start < header_end)
                {
                    std::string::size_type line_end = body.find("\r\n", line_start);
                    std::string::size_type delim = body.find(": ", line_start);
                    m_answer.m_headers.push_back((http::Header){
                        body.substr(line_start, delim - line_start),
                        body.substr(delim + 2, line_end - delim - 2)});
                    line_start = line_end + 2;
                }
                m_answer.m_status_code = 200;
            }
        }
        m_state = e_response_ready;
        return true;
    }
    return false;
}

bool ft::Client::cgi_write()
{
    return true;
}
