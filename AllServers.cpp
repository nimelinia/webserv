//
// Created by Streetmentioner Copycat on 5/22/21.
//

#include "AllServers.hpp"


ft::AllServers::AllServers(Config &config) : m_config(config)
{
	for (int i = 0; i < m_config.count_servers; ++i)
	{
		Server _server(m_config, config.port[i], config.hostaddress[i], i);
		m_servers.push_back(_server);
	}
}

ft::AllServers::~AllServers()
{
}

bool ft::AllServers::start_all_servers()
{
	for (int i = 0; i < m_config.count_servers; ++i)
	{
		if (!m_servers[i].create_server())
			return (false);
	}

	int		max_fd;
	fd_set	clients;

	FD_ZERO(&clients);
	max_fd = 3;
	for (int i = 0; i < m_config.count_servers; ++i)
	{
		FD_SET(m_servers[i].getMSocketFd(), &clients);																// ставлю соответствующий флажок в элементе массива файловых дескрипторов
		if (m_servers[i].getMSocketFd() > max_fd)
			max_fd = m_servers[i].getMSocketFd();
	}


	for (;;)
	{
		struct sockaddr *addr_client;
		socklen_t addrlen = sizeof(*addr_client);
		int fd;																												// дескриптор клиента
		fd_set readfds;
		fd_set writefds;
		fd_set exfds;
		FD_COPY(&clients, &readfds);
		FD_COPY(&clients, &writefds);
		FD_COPY(&clients, &exfds);

		struct timeval tv;
		tv.tv_sec = 1;
		tv.tv_usec = 0;

		int res;
		std::cout << "Select()" << std::endl;
		if ((res = select(max_fd + 1, &readfds, &writefds, &exfds, &tv)) == -1)												// т.к. таймауты нулевые, селект просто проверяет
																															// состояние сокетов для записи, выставляет номера фдшников в массиве и выходит
		{
			return (false);																									// прописать обработку ошибок
		}
		if (res == 0)
			continue;
		std::cout << "Socket loop" << std::endl;
		for (int i = 0; i < m_open_sockets.size(); ++i) {
			if (FD_ISSET(m_open_sockets[i], &readfds))
			{
				std::cout << "сокет " << m_open_sockets[i] << " открыт для чтения" << std::endl; 							// функция чтения из сокета
				this->read_from_socket(m_open_sockets[i]);
			}
			if (FD_ISSET(m_open_sockets[i], &writefds))
			{
				std::cout << "сокет " << m_open_sockets[i] << " открыт для записи" << std::endl;
				this->write_to_socket(m_open_sockets[i]);
			}
			if (FD_ISSET(m_open_sockets[i], &exfds)) 																	// функция записи в сокет
				std::cout << "сокет " << m_open_sockets[i] << " содержит исключение" << std::endl;							// обработчик ошибок
		}
		for (int i = 0; i < m_config.count_servers; ++i)
		{
			if (FD_ISSET(m_servers[i].getMSocketFd(), &readfds))
			{
				std::cout << "Server socket" << std::endl;
				int connect_fd = accept(m_servers[i].getMSocketFd(), addr_client, &addrlen);
				if (connect_fd < 0)
				{
					std::cout << "Ошибка: принятия" << std::endl;
					close(m_servers[i].getMSocketFd());																		// тут из программы не выходим, видимо
				}
				else
				{
					std::cout << "Появилось новое подключение" << std::endl;
					m_open_sockets.push_back(connect_fd);
					FD_SET(connect_fd, &clients);
					fcntl(connect_fd, F_SETFL, O_NONBLOCK);																	// ставлю сокет в неблокирующий режим.
					max_fd = connect_fd;
				}
			}
		}
		usleep(1000);
	}
	return (true);
}

ssize_t	ft::AllServers::read_from_socket(int fd)
{
	char	buff[1567415];
	ssize_t	ret;

	ret = read(fd, buff, sizeof(buff));
	if (ret == 0)
		return (0);
	if (ret == -1)
		return (-1);


	return 0;
}

ssize_t ft::AllServers::write_to_socket(int fd)
{
	return 0;
}


