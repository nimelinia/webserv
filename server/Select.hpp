//
// Created by Streetmentioner Copycat on 5/22/21.
//

#ifndef SELECT_HPP
#define SELECT_HPP

#include "util/Singleton.hpp"
#include "webserv.hpp"

namespace ft {
class Select : public util::Singleton<Select>
{
public:
	bool	can_read(int socket) const;
	bool	can_write(int socket) const;
	bool	update();
	void	set_fd(int socket);
	void	clear_fd(int socket);
	void	set_max_fd(int max_fd);

			Select();

public:
	fd_set	m_clients;
	fd_set	m_read;
	fd_set	m_write;
	int		m_max_fd;

};
}

#endif //SELECT_HPP
