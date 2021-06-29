//
// Created by Streetmentioner Copycat on 5/22/21.
//

#pragma once
//#ifndef ALLSERVERS_HPP
//#define ALLSERVERS_HPP

#include <iostream>
#include <vector>
#include "webserv.hpp"
#include "Server.hpp"
#include "Select.hpp"

struct	Config;
class	Client;
class	Message;

namespace ft
{
	class	Server;
	class AllServers
	{
	public:
		AllServers();
		virtual ~AllServers();
		void	create_server(Host &hosts);
		bool	start_all_servers();
		int 	find_max_fd();

	private:
		std::list<Server>		m_servers;


	};
}

//#endif //ALLSERVERS_HPP
