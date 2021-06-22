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
		m_msg.parse(m_buff);
		std::pair<http::RequestParser::EResult, size_t> res = m_parser.parse(m_msg);
		if (res.first == http::RequestParser::EOk)
			m_state = e_read_body;
		else if (res.first == http::RequestParser::EError)
		{
			m_state = e_error;
			m_msg.m_bad_request = true;
			m_msg.m_error_num = 400;
		}
	}
	if (!m_clients_data[index].m_msg.m_bad_request &&
		m_clients_data[index].m_msg.m_read <= m_clients_data[index].m_server->getMLimitBodySize())
	{
//		m_clients_data[index].m_msg.copy_buff(buff);																								// скопировала прочтеное в мессадж
		m_clients_data[index].m_msg.parse(buff);
		m_clients_data[index].m_answer->generate_answer(m_clients_data[index].m_msg);
	}
	clean_buf(buff);																											// чищу буфер (сомнительно, что это работает)
	if (m_clients_data[index].m_msg.m_read > m_clients_data[index].m_server->getMLimitBodySize() &&
		!m_clients_data[index].m_msg.m_error_num)
		m_clients_data[index].m_msg.m_error_num = 413;
	return 0;

	return false;
}

bool ft::Client::send_message()
{
	return false;
}

void ft::Client::close()
{
	::close(m_socket_cl);																						// закрываю сокет
	Select::get().clear_fd(m_socket_cl);
}
