//
// Created by Streetmentioner Copycat on 5/22/21.
//

#include "Select.hpp"
#include <sys/select.h>

ft::Select::Select() : m_max_fd(2)
{
	FD_ZERO(&m_clients);
}

void ft::Select::set_max_fd(int max_fd)
{
	m_max_fd = max_fd;
}

bool ft::Select::can_read(int socket) const
{
	return (FD_ISSET(socket, &m_read) != 0);
}

bool ft::Select::can_write(int socket) const
{
	return (FD_ISSET(socket, &m_write) != 0);
}

bool ft::Select::update()
{
    std::memcpy(&m_read, &m_clients, sizeof(m_clients));
    std::memcpy(&m_write, &m_clients, sizeof(m_clients));
	struct timeval tv;
	tv.tv_sec = 3;
	tv.tv_usec = 0;

	int res;
	if ((res = select(m_max_fd + 1, &m_read, &m_write, 0, &tv)) == -1)
		throw std::runtime_error("select error: " + std::string(strerror(errno)));
	return (res != 0);
}

void ft::Select::set_fd(int socket)
{
	FD_SET(socket, &m_clients);
}

void ft::Select::clear_fd(int socket)
{
	FD_CLR(socket, &m_clients);
}
