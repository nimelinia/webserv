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
	m_answer(),
//	m_answer(&server->m_config),
//	m_parser(server->m_config.limit_body_size)																				// тут нужно поменять, т.к. этот лимит лежит внутри location и мы сначала должны распарсить uri, а потом уже определять размер
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
//	ResponseHandler handler(m_server->m_config, *this);
	if (m_state == e_request_parse)
	{
        http::RequestParser::EResult res = m_parser.parse(m_msg, m_buff, ret);
        if (res == http::RequestParser::EParse)
			return false;
		std::vector<http::Header>::iterator it = std::find_if(m_msg.m_headers.begin(),
															  m_msg.m_headers.end(),
															  http::FindHeader("host"));
		if (it != m_msg.m_headers.end())
			m_msg.host_name = it->value;
		else
			res = http::RequestParser::EError;
		if (res == http::RequestParser::EOk)
		{
			LOGD << "URI: " << m_msg.m_uri;
			LOGD << "Method: " << m_msg.m_method;
			LOGD << "Headers:";
			for (std::vector<http::Header>::iterator hit = m_msg.m_headers.begin(); hit != m_msg.m_headers.end(); ++hit)
				LOGD << "\t" << hit->name << ": " << hit->value;
			LOGD;
			m_state = e_request_ready;
		}
		else if (res == http::RequestParser::EError)
		{
			m_answer.m_status_code = 400;																					// брать номер из msg
			m_state = e_request_ready;
		}
	}
	if (m_state == e_request_ready)
	{
		const std::string host = m_msg.host_name.substr(0, m_msg.host_name.find_last_of(':'));
		std::list<Config>::iterator it = m_server->m_config.begin();
		for (; it != m_server->m_config.end(); ++it)
		{
			if (it->server_name == host)
				break;
		}
		if (it == m_server->m_config.end())
			it = m_server->m_config.begin();

		ResponseHandler handler(*it, *this);
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

//void ft::Client::read_body(ssize_t readed)
//{
//	if (readed > m_content_length)
//	{
//		m_msg.m_body.append(m_buff, m_parsed, m_content_length);
//		m_content_length = 0;
//		m_state = e_request_ready;
//	}
//	else
//	{
//		m_msg.m_body.append(m_buff, m_parsed, readed);
//		m_content_length -= readed;
//	}
//	if (m_parsed)
//		m_parsed = 0;
//}

void ft::Client::close()
{
	::close(m_socket_cl);																						// закрываю сокет
	Select::get().clear_fd(m_socket_cl);
}

//void ft::Client::find_content_length()
//{
//	std::vector<http::Header>::iterator it = std::find_if(m_msg.m_headers.begin(),
//														  m_msg.m_headers.end(),
//														  http::FindHeader("Content-Length"));
//	if (it == m_msg.m_headers.end())
//	{
//		m_state = e_request_ready;
//		return;
//	}
//	m_content_length = std::strtoul(it->value.c_str(), 0, 0);
//	if (!m_content_length && it->value != "0")
//	{
////		m_msg.m_bad_request = true;
//		m_msg.m_error_num = 400;
//	}
//	if (m_content_length > m_server->m_config.limit_body_size)
//	{
//		m_msg.m_error_num = 413;
//		m_state = e_error;
//	}
//}

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
    ssize_t ret = ::read(m_cgi_process.read_fd(), m_buff, BUFFER_SIZE);
    if (ret == 0 || ret == -1)
    {
        ::close(m_cgi_process.read_fd());
        Select::get().clear_fd(m_cgi_process.read_fd());
        m_cgi_process.end_read(ret);

        if (m_cgi_process.state() == http::CgiProcess::EError)
            m_answer.m_status_code = 500;
        else
        {
            http::CgiHandler handler(m_server->m_config, *this, Uri());
            handler.parse_cgi_body();
        }
        m_state = e_response_ready;
        return true;
    }
    else
    {
        m_answer.m_body.append(m_buff, ret);
        return false;
    }
}

bool ft::Client::cgi_write()
{
    return true;
}
