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

namespace ft
{
	class AllServers
	{
	public:
		AllServers(Config &config);
		virtual ~AllServers();
		bool	start_all_servers();

	private:
		Config				&m_config;
		std::vector<Server>	m_servers;
		std::vector<int>	m_open_sockets;

		AllServers();
	};
}

#endif //ALLSERVERS_HPP
