//
// Created by Streetmentioner Copycat on 5/22/21.
//

#pragma once
//#ifndef ALLSERVERS_HPP
//#define ALLSERVERS_HPP

#include <iostream>
#include <vector>
#include "webserv.hpp"
//#include "Message.hpp"
#include "Server.hpp"
#include "Client.hpp"

struct	Config;
class	Client;
class	Message;

namespace ft
{
	class	Server;
	class AllServers
	{
	public:
		AllServers(Config &config);
		virtual ~AllServers();
		bool	start_all_servers();
		ssize_t	read_from_socket(int index);
		ssize_t	write_to_socket(int index);
		int find_max_fd();
		void	clean_buf(char *buff);

	private:
		Config					&m_config;
		std::vector<Server>		m_servers;
		std::vector<int>		m_open_sockets;
		std::vector<Client>		m_clients_data;
		fd_set					m_clients;

		AllServers();
	};
}

//#endif //ALLSERVERS_HPP
