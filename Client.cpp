//
// Created by Streetmentioner Copycat on 5/8/21.
//

#include "Client.hpp"
#include "Server.hpp"
#include "http/RequestParser.h"


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
	m_msg.m_read += ret;
	if (m_state == e_parse_header)
	{
		std::pair<http::RequestParser::EResult, size_t> res = m_parser.parse(m_msg, m_buff, BUFFER_SIZE);
		m_msg.m_parsed += res.second;
		if (res.first == http::RequestParser::EOk)
			m_state = e_read_body;
		else if (res.first == http::RequestParser::EError)
		{
			m_state = e_error;
			m_msg.m_bad_request = true;
			m_msg.m_error_num = 400;
		}
	}
	if (m_state == e_read_body)
	{
		if (read_body())
			m_state = e_ready;
		if (m_msg.m_error_num)
			m_state = e_error;
		m_msg.m_parsed = 0;
	}
	return false;
}

bool ft::Client::send_message()
{
	return false;
}

bool ft::Client::read_body()
{
	if (!m_msg.m_body_tale)
	{
		for (size_t i = 0; i < m_msg.m_headers.size(); ++i) {
			if (m_msg.m_headers[i].name == "Content-Length") {
				m_msg.m_body_tale = std::strtoul(m_msg.m_headers[i].value.c_str(), 0, 0);
				if (!m_msg.m_body_tale && m_msg.m_headers[i].value != "0") {
					m_msg.m_bad_request = true;
					m_msg.m_error_num = 400;
					return (false);
				}
				break;
			}
			if (i == m_msg.m_headers.size() - 1)
				return (true);
		}
		if (m_msg.m_body_tale > m_server->m_config.limit_body_size)
		{
			m_msg.m_error_num = 413;
			return (false);
		}
	}
	if (m_msg.m_read - m_msg.m_parsed < m_msg.m_body_tale)
	{
		m_msg.m_body.append(m_buff, m_msg.m_parsed, m_msg.m_read - m_msg.m_parsed);
		m_msg.m_body_tale -= m_msg.m_read - m_msg.m_parsed;
		return (false);
	}
	else
		m_msg.m_body.append(m_buff, m_msg.m_parsed, m_msg.m_body_tale);
	return (true);
}

void ft::Client::close()
{
	::close(m_socket_cl);																						// закрываю сокет
	Select::get().clear_fd(m_socket_cl);
}
