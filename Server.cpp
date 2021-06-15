//
// Created by Streetmentioner Copycat on 5/22/21.
//

#include "Server.hpp"

ft::Server::Server(Config &config, size_t port, char *host_address, size_t id) :
	m_config(config),
	m_port(port),
	m_host_address(host_address),
	m_id(id),
	m_socket_fd(0),
	m_error_fatal(false)
{
	if (config.limit_body_size[id])
		m_limit_body_size = config.limit_body_size[m_id];
	else
		m_limit_body_size = MAXBODYSIZE;
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
	addr.sin_addr.s_addr = inet_addr(m_host_address); 																		// заносит четырехбайтное целое число, отображающее корректный
																															// адрес, представленный в виде строки, в структуру.
																															// возвращает адрес в сетевом порядке байтов

	int opt = 1;
	setsockopt(m_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)); 													//  чтобы предотправить "залипание" порта
	fcntl(m_socket_fd, F_SETFL, O_NONBLOCK); 																				// ставлю сокет в неблокирующий режим.
	if (bind(m_socket_fd, (struct sockaddr*) &addr, sizeof (addr)) == -1) 													// снабжаем сокет адресом
	{
		std::cout << "Ошибка: связывания сервера с id " << m_id << std:: endl;
		close(m_socket_fd); 																								// возможно, это лучше перенести в деструктор
		m_error_fatal = true;
		return (false);
	}
	if (listen(m_socket_fd, -1) == -1) 																						// устанавливаю сокет сервера в слушающий режим - второй аргумент - размер
																															// очереди (-1 значит, что по максимуму, предусмотренному для системы -
																															// цифра меняется на максимально поддерживаемое системой)
	{
		std::cout << "Ошибка: прослушивания сервера с id " << m_id << std::endl;
		close(m_socket_fd);
		m_error_fatal = true;
		return (false);
	}
	return (true);
}

int ft::Server::getMSocketFd() const
{
	return m_socket_fd;
}

size_t ft::Server::getMLimitBodySize() const
{
	return m_limit_body_size;
}

