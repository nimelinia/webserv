#include "webserv.hpp"

/*
* начинаем работу с сокетом
*/

void	work_with_socket(sockaddr_in *addr, int *socket_fd, Config *config)
{
	Message message;
	int		fd_read;
	int		fd_write;

	if (listen(*socket_fd, -1) == -1)
	{
		std::cout << "Ошибка: прослушивания" << std::endl;
		close(*socket_fd);
		exit(EXIT_FAILURE);
	}
	for (;;)
	{
		int connect_fd = accept(*socket_fd, 0, 0);
		fd_set fd;
		FD_ZERO(&fd); // снимаю все флаги в массиве файловых дескрипторов
		if (connect_fd < 0)
		{
			std::cout << "Ошибка: принятия" << std::endl;
			close(*socket_fd);
			exit(EXIT_FAILURE);
		}
		struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = 0;
		fcntl(*socket_fd, F_SETFL, O_NONBLOCK); // не понимаю, что делаю - просто скопировала из сабджекта
		FD_SET(*socket_fd, &fd); // ставлю соответствующий флажок в элементе массива файловых дескрипторов
		if (select(*socket_fd + 1, NULL, &fd, NULL, &tv) == -1) // т.к. таймауты нулевые, селект просто проверяет
			// состояние сокетов для записи, выставляет номера фдшников в массиве и выходит
		{
			exit(getdate_err); // тут нужно выходить с кодом последней ошибки, например
		}
		if ((fd_write = FD_ISSET(*socket_fd, &fd)))
			// что-то делаю
		if (select(*socket_fd + 1, &fd, NULL, NULL, &tv) == -1) // проверяю состояние сокетов для чтения
		{
			exit(getdate_err); // тут нужно выходить с кодом последней ошибки, например
		}
		if ((fd_read = FD_ISSET(*socket_fd, &fd)))
		{
			size_t ret = read(fd_read, (void*)message.input.c_str(), 1024);
			while (ret)
			{
				ret = read(fd_read, (void*)message.buf.c_str(), 1024);
				message.input = message.input + message.buf;
			}
			parse_message(&message); // отправила Яне в парсер данные, считанные из сокета
			interpret_message(&message);
			send_response(fd_read, &message);
		}
		close(connect_fd);
	}
}

/*
* связываем сокет сервера с IP-адресом и портом, содержащимися в переданном нами confige-файле
*/

void	connect_to_socket(sockaddr_in *addr, int *socket_fd)
{
	if (bind(*socket_fd, (struct sockaddr*) addr, sizeof (*addr)) == -1)
	{
		std::cout << "Ошибка: связывания" << std:: endl;
		close(*socket_fd);
		exit(EXIT_FAILURE);
	}
}

void	fill_addr(sockaddr_in *addr, Config config)
{
	(*addr).sin_family = PF_INET;
	(*addr).sin_port = htons(config.port);
	(*addr).sin_addr.s_addr = inet_addr(config.localhost);
}

void	create_socket(int *socket_fd)
{
	*socket_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (*socket_fd == -1)
	{
		std::cout << "ошибка при создании сокета" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void	check_count_arguments(int argc)
{
		if (argc != 2)
	{
		std::cout << "There is no config for server!" << std::endl;
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char **argv)
{
	struct sockaddr_in	addr;
	int					socket_fd;
	Config				config;

	check_count_arguments(argc);
	create_socket(&socket_fd);
	pars_config(argv[1], &config); // функция парсинга конфига сервера в структуру
	fill_addr(&addr, config);
	connect_to_socket(&addr, &socket_fd);
	work_with_socket(&addr, &socket_fd, &config);

	return (0);
}
