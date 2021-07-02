//
// Created by Streetmentioner Copycat on 5/22/21.
//

#pragma once

//#ifndef SERVER_HPP
//#define SERVER_HPP

#include <iostream>
#include <vector>
#include <list>
#include "Client.hpp"
#include "Select.hpp"

#define	MAXBODYSIZE		10240;

struct Config;

namespace ft
{
	class Server
	{
	public:
		Server(Host& host, size_t port, std::string &host_address);
		virtual ~Server();

		bool				do_work();
		bool				create_server();
		int					get_max_fd() const;
		bool 				create_new_connection();

		std::list<Config>&	m_configs;
		std::list<Client>	m_clients;
	private:
		size_t				m_port;
		std::string 		m_host_address;
		int					m_socket_fd;
		bool				m_error_fatal;
		int 				m_max_fd;
		struct sockaddr 	m_addr_client;

		Server();

	};
}

//#endif //SERVER_HPP
