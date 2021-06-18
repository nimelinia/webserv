//
// Created by Streetmentioner Copycat on 5/22/21.
//

#pragma once

//#ifndef SERVER_HPP
//#define SERVER_HPP

#include <iostream>
#include <vector>
#include "webserv.hpp"

#define	MAXBODYSIZE		10240;

struct Config;

namespace ft
{
	class Server
	{
	public:
		Server(Config &config, size_t port, char *host_address, size_t id);
		virtual ~Server();

		bool				create_server();
		int					getMSocketFd() const;

		size_t				getMLimitBodySize() const;

		Config				&m_config;
	private:
		size_t				m_port;
		char 				*m_host_address;
		size_t				m_id;
		int					m_socket_fd;
		bool				m_error_fatal;
		size_t				m_limit_body_size;

		Server();

	};
}

//#endif //SERVER_HPP
