//
// Created by Streetmentioner Copycat on 5/8/21.
//

#include "Client.hpp"
#include "Server.hpp"
#include "http/RequestParser.h"
#include "ResponseHandler.hpp"


ft::Client::Client(int socketCl, Server *server) :
	m_state(e_parse_header),
	m_socket_cl(socketCl),
	m_server(server),
	m_answer(&server->m_config)
{
	m_buff = new char[BUFFER_SIZE];
}

bool ft::Client::read_message()
{
	ssize_t	ret;

	ret = recv(m_socket_cl, m_buff, BUFFER_SIZE, 0);
	if (ret == 0 || ret == -1)
		return (true);
	if (m_state == e_parse_header)
	{
		std::pair<http::RequestParser::EResult, size_t> res = m_parser.parse(m_msg, m_buff, ret);
		m_parsed = res.second;
		if (res.first == http::RequestParser::EOk)
		{
			m_state = e_read_body;
			find_content_length();
			ret -= static_cast<ssize_t>(res.second);
		}
		else if (res.first == http::RequestParser::EError)
		{
			m_state = e_error;
			m_msg.m_error_num = 400;
		}
	}
	if (m_state == e_read_body)
	{
		read_body(ret);
	}
	if (m_state == e_request_ready)
	{
		m_parser.reset();
		ResponseHandler handler(m_server->m_config, m_msg, m_answer);
		if (handler.generate_answer())
		{
			handler.generate_status_body();
			m_state = e_response_ready;
		}
	}
	return (false);
}

bool ft::Client::send_message()
{
 	m_answer.create_final_response();
	ssize_t	ret;
	ret = send(m_socket_cl, m_answer.m_final_response.c_str(),  m_answer.m_size_response, 0);
	if (ret == 0 || ret == -1)
		return (true);
	m_answer.m_final_response.erase(0, ret);
	if (m_answer.m_final_response.empty())																				// если не доотправлено, то в следующий раз по флажку пойдет отправлять
	{
		m_msg.m_ready_responce = false;
		m_msg.clean();
		m_answer.clean();
		m_parser.reset();
		m_state = e_parse_header;
	}
	return (false);
}

void ft::Client::read_body(ssize_t readed)
{
	if (static_cast<size_t>(readed) > m_content_length)
	{
		m_msg.m_body.append(m_buff, m_parsed, m_content_length);
		m_content_length = 0;
		m_state = e_request_ready;
	}
	else
	{
		m_msg.m_body.append(m_buff, m_parsed, readed);
		m_content_length -= readed;
	}
	if (m_parsed)
		m_parsed = 0;
}

void ft::Client::close()
{
	::close(m_socket_cl);																						// закрываю сокет
	Select::get().clear_fd(m_socket_cl);
}

void ft::Client::find_content_length()
{
	std::vector<http::Header>::iterator it = std::find_if(m_msg.m_headers.begin(),
														  m_msg.m_headers.end(),
														  http::FindHeader("Content-Length"));
	if (it == m_msg.m_headers.end())
	{
		m_state = e_request_ready;
		return;
	}
	m_content_length = std::strtoul(it->value.c_str(), 0, 0);
	if (!m_content_length && it->value != "0")
	{
//		m_msg.m_bad_request = true;
		m_msg.m_error_num = 400;
	}
	if (m_content_length > m_server->m_config.limit_body_size)
	{
		m_msg.m_error_num = 413;
		m_state = e_error;
	}
}

bool ft::Client::ready_write() const
{
	return (m_state == e_response_ready);
}
