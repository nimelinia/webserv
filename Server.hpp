//
// Created by Streetmentioner Copycat on 5/22/21.
//

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>
#include "webserv.hpp"

struct Config;

namespace ft
{
	class Server
	{
	public:
		Server(Config &config, size_t port, char *host_address, size_t id);
		virtual ~Server();

		bool			create_server();
		virtual bool	start_server();																						// или это перенести это в класс AllServers
		int				getMSocketFd() const;

	private:
		Config				&m_config;
		size_t				m_port;
		char 				*m_host_address;
		size_t				m_id;
		int					m_socket_fd;
		bool				m_error_fatal;
		std::vector<int>	m_open_sockets;

		Server();

	};
}

#endif //SERVER_HPP
