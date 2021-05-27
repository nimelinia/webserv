//
// Created by Streetmentioner Copycat on 5/22/21.
//

#ifndef ALLSERVERS_HPP
#define ALLSERVERS_HPP

#include <iostream>
#include <vector>
#include "webserv.hpp"

struct	Config;
class	Server;
class	Client;

namespace ft
{
	class AllServers
	{
	public:
		AllServers(Config &config);
		virtual ~AllServers();
		bool	start_all_servers();
		ssize_t	read_from_socket(int fd);
		ssize_t	write_to_socket(int fd);

	private:
		Config					&m_config;
		std::vector<&Server>		m_servers;
		std::vector<int>		m_open_sockets;
		std::vector<&Client>	m_clients_data;

		AllServers();
	};
}

#endif //ALLSERVERS_HPP
