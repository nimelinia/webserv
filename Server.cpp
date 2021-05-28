//
// Created by Streetmentioner Copycat on 5/22/21.
//

#include "Server.hpp"

ft::Server::Server(Config &config, size_t port, char *host_address, size_t id) :
	m_config(config),
	m_port(port),
	m_host_address(host_address),
	m_id(id),
	m_error_fatal(false),
	m_socket_fd(0)
{
	if (config.limit_body_size)
		m_limit_body_size = config.limit_body_size;
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

//bool ft::Server::start_server()
//{
//	int		max_fd;
//	fd_set	clients;
//
//	FD_ZERO(&clients);
//	FD_SET(m_socket_fd, &clients);																						// ставлю соответствующий флажок в элементе массива файловых дескрипторов
//	max_fd = m_socket_fd;
//
//	for (;;)
//	{
//		struct sockaddr *addr_client;
//		socklen_t addrlen = sizeof(*addr_client);
//		int fd;																												// дескриптор клиента
//		fd_set readfds;
//		fd_set writefds;
//		fd_set exfds;
//		FD_COPY(&clients, &readfds);
//		FD_COPY(&clients, &writefds);
//		FD_COPY(&clients, &exfds);
//
//		struct timeval tv;
//		tv.tv_sec = 1;
//		tv.tv_usec = 0;
//
//		int res;
//		std::cout << "Select()" << std::endl;
//		if ((res = select(max_fd + 1, &readfds, &writefds, &exfds, &tv)) == -1)												// т.к. таймауты нулевые, селект просто проверяет
//																															// состояние сокетов для записи, выставляет номера фдшников в массиве и выходит
//		{
//			return (false);																									// прописать обработку ошибок
//		}
//		if (res == 0)
//			continue;
//		std::cout << "Socket loop" << std::endl;
//		for (int i = 0; i < m_open_sockets.size(); ++i) {
//			if (FD_ISSET(m_open_sockets[i], &readfds))
//				std::cout << "сокет " << m_open_sockets[i] << " открыт для чтения"
//						  << std::endl; 																					// функция чтения из сокета
//			if (FD_ISSET(m_open_sockets[i], &writefds))
//				std::cout << "сокет " << m_open_sockets[i] << " открыт для записи" << std::endl;
//			if (FD_ISSET(m_open_sockets[i], &exfds)) 																	// функция записи в сокет
//				std::cout << "сокет " << m_open_sockets[i] << " содержит исключение" << std::endl;							// обработчик ошибок
//		}
//
//		if (FD_ISSET(m_socket_fd, &readfds)) {
//			std::cout << "Server socket" << std::endl;
//			int connect_fd = accept(m_socket_fd, addr_client, &addrlen);
//			if (connect_fd < 0) {
//				std::cout << "Ошибка: принятия" << std::endl;
//				close(m_socket_fd);																							// тут из программы не выходим, видимо
//			} else {
//				std::cout << "Появилось новое подключение" << std::endl;
//				m_open_sockets.push_back(connect_fd);
//				FD_SET(connect_fd, &clients);
//				fcntl(connect_fd, F_SETFL, O_NONBLOCK);																		// ставлю сокет в неблокирующий режим.
//				max_fd = connect_fd;
//			}
//		}
//		usleep(1000);
//	}
//}

int ft::Server::getMSocketFd() const
{
	return m_socket_fd;
}

size_t ft::Server::getMLimitBodySize() const
{
	return m_limit_body_size;
}
