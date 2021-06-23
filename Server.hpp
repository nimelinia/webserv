//
// Created by Streetmentioner Copycat on 5/22/21.
//

#pragma once

//#ifndef SERVER_HPP
//#define SERVER_HPP

#include <iostream>
#include <vector>
#include "webserv.hpp"
#include "Client.hpp"
#include "Select.hpp"

#define	MAXBODYSIZE		10240;

struct Config;

namespace ft
{
	class Server
	{
	public:
		Server(Config config, size_t port, std::string &host_address);
		virtual ~Server();

		bool				do_work();
		bool				create_server();
		int					get_max_fd() const;
		bool 				create_new_connection();

//		size_t				get_m_limit_body_size() const;

		Config				m_config;
		std::list<Client>	m_clients;
	private:
		size_t				m_port;
		std::string 		m_host_address;
//		size_t				m_id;
		int					m_socket_fd;
		bool				m_error_fatal;
		size_t				m_limit_body_size;
		int 				m_max_fd;
		struct sockaddr 	m_addr_client;

		Server();

	};
}

//#endif //SERVER_HPP
