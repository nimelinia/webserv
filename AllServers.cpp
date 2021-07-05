//
// Created by Streetmentioner Copycat on 5/22/21.
//

#include "AllServers.hpp"
#include "log/Log.h"

ft::AllServers::AllServers()
{
}

ft::AllServers::~AllServers()
{
	// закрыть все серваки
}


void ft::AllServers::create_server(Host &host)
{
	ft::Server _server(host, host.configs.front().port, host.configs.front().hostaddress);
	m_servers.push_back(_server);
}


bool ft::AllServers::start_all_servers()
{
	for (std::list<Server>::iterator it = m_servers.begin(); it != m_servers.end(); ++it)
	{
		if (!it->create_server())
			return (false);
	}
	Select::get().set_max_fd(find_max_fd());

	for (;;)
	{
		if (Select::get().update())
		{
			bool update_max_fd = false;
			for (std::list<Server>:: iterator it = m_servers.begin(); it != m_servers.end(); ++it)
			{
				if (it->do_work())
					update_max_fd = true;
			}
			if (update_max_fd)
				Select::get().set_max_fd(find_max_fd());
		}
		else
		    LOGD << "Waiting. Max descriptor: " << Select::get().m_max_fd;
        usleep(1000);
	}
	return (true);
}

int ft::AllServers::find_max_fd()
{
	int max_fd = 2;
	for (std::list<Server>::iterator max = m_servers.begin(); max != m_servers.end(); ++max)
		max_fd = std::max(max_fd, max->get_max_fd());
	return (max_fd);
}