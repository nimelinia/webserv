//
// Created by Streetmentioner Copycat on 5/22/21.
//

#include "Server.hpp"
#include "webserv.hpp"
#include "log/Log.hpp"

ft::Server::Server(Host& host, size_t port, std::string &host_address) :
	m_configs(host.configs),
	m_port(port),
	m_host_address(host_address),
	m_socket_fd(-1),
	m_error_fatal(false),
	m_timeout(42)
{
}

ft::Server::~Server()
{

}

bool ft::Server::create_server()
{
	struct sockaddr_in	addr;
	this->m_socket_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); 															// первый параметр - это семейство адресации - у нас TCP/IP,
																															// второй - тип взаимодействия (у нас потоковое, для
																															// дейтаграммного нужно SOCK_DGRAM) и указываем тип протокола - у нас TCP
	if (this->m_socket_fd == -1)
	{
		std::cout << "ошибка при создании сокета" << std::endl;
		m_error_fatal = true; 																								// заменить на выход с ошибкой и вылетать с ней из мейника
		return (false);
	}
	addr.sin_family = PF_INET;
	addr.sin_port = htons(m_port); 																							// преобразую в архитектуру сервера
	addr.sin_addr.s_addr = inet_addr(m_host_address.c_str()); 																// заносит четырехбайтное целое число, отображающее корректный
																															// адрес, представленный в виде строки, в структуру.
																															// возвращает адрес в сетевом порядке байтов

	int opt = 1;
	setsockopt(m_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)); 													//  чтобы предотправить "залипание" порта
	fcntl(m_socket_fd, F_SETFL, O_NONBLOCK); 																				// ставлю сокет в неблокирующий режим.
	if (bind(m_socket_fd, (struct sockaddr*) &addr, sizeof (addr)) == -1) 													// снабжаем сокет адресом
	{
		std::cout << "Ошибка: связывания сервера" << std:: endl;
		close(m_socket_fd); 																								// возможно, это лучше перенести в деструктор
		m_error_fatal = true;
		return (false);
	}
	if (listen(m_socket_fd, -1) == -1) 																						// устанавливаю сокет сервера в слушающий режим - второй аргумент - размер
																															// очереди (-1 значит, что по максимуму, предусмотренному для системы -
																															// цифра меняется на максимально поддерживаемое системой)
	{
		std::cout << "Ошибка: прослушивания сервера" << std::endl;
		close(m_socket_fd);
		m_error_fatal = true;
		return (false);
	}
	Select::get().set_fd(m_socket_fd);
	return (true);
}

bool ft::Server::do_work()
{
	bool need_update = false;
	std::list<Client>::iterator it = m_clients.begin();
	while (it != m_clients.end())
	{
		it->check_cgi();
		if (Select::get().can_write(it->m_socket_cl) && it->ready_write())
		{
            if (it->send_message())
			{
				it->close();
				m_clients.erase(it++);
				need_update = true;
				LOGD << "Total connections: " << m_clients.size();
				continue;
			}
		}
		if (Select::get().can_read(it->m_socket_cl) && it->ready_read())
		{
			if (it->read_message())
			{
				it->close();
				m_clients.erase(it++);
				need_update = true;
				LOGD << "Total connections: " << m_clients.size();
				continue;
			}
		}
		if ((time_t)std::difftime(std::time(NULL), it->m_last_action_time) > m_timeout)
		{
			it->close();
			m_clients.erase(it++);
			need_update = true;
			LOGD << "Connection timeout";
			LOGD << "Total connections: " << m_clients.size();
			continue;
		}
		++it;
	}
	if (Select::get().can_read(m_socket_fd))
		need_update = create_new_connection() || need_update;
	return (need_update);
}


int ft::Server::get_max_fd() const
{
	int max_fd = m_socket_fd;
	for (std::list<Client>::const_iterator max = m_clients.begin(); max != m_clients.end(); ++max)
		max_fd = std::max(max_fd, max->max_fd());
	return (max_fd);
}

bool ft::Server::create_new_connection()
{
	socklen_t addrlen = sizeof(m_addr_client);
	int connect_fd = accept(m_socket_fd, &m_addr_client, &addrlen);
	if (connect_fd < 0)
	{
		std::cout << "Ошибка: принятия" << std::endl;
		return (false);
	}

	else
	{
        struct sockaddr_in peer;
        socklen_t peer_len  = sizeof(peer);
        ::getpeername(connect_fd, (struct sockaddr*)&peer, &peer_len);
        LOGD << "New connection (" << ::inet_ntoa(peer.sin_addr) << ":"
                << peer.sin_port << ")";
		Select::get().set_fd(connect_fd);
		fcntl(connect_fd, F_SETFL, O_NONBLOCK);																				// ставлю сокет в неблокирующий режим.
		Client	new_client(connect_fd, this);
		m_clients.push_back(new_client);
		m_clients.back().init_buffer();

		LOGD << "Total connections: " << m_clients.size();

		return (true);
	}
}



