#include "webserv.hpp"

void	check_readable_message_body(Message *message)
{
	// !!! нужно учесть!!!
	// Пользовательский агент ДОЛЖЕН отправить Content-Length в сообщении запроса, когда не передано
	// Transfer-Encoding и метод запроса определяет значение для вложенного тела полезной нагрузки.
	// Например, поле заголовка Content-Length обычно отправляется в запросе POST, даже если значение
	// равно 0 (указывает на пустое тело полезной нагрузки). Пользовательский агент НЕ ДОЛЖЕН отправлять
	// поле заголовка Content-Length, если сообщение запроса не содержит тела полезной нагрузки,
	// а семантика метода не ожидает такого тела.

	if (message->fields.find("Content-Length") != message->fields.end() ||
		message->fields.find("Transfer-Encoding") != message->fields.end())
		message->readable_body = true;
	else
		message->readable_body = false;
}

void	check_http_version(Message *message)
{
	if (message->http_version > 1.1)
		message->wrong_version = true;
	else
		message->wrong_version = false;
}

void	interpret_message(Message *message)
{
	check_http_version(message);
	check_readable_message_body(message);

}

/*
* начинаем работу с сокетом
*/

void	work_with_socket(sockaddr_in *addr, int *socket_fd, Config *config)
{
	Message message;
	int		fd_read;
	int		fd_write;

	if (listen(*socket_fd, -1) == -1) // устанавливаю сокет сервера в слушающий режим - второй аргумент - размер очереди (-1 значит, что по максимуму, предусмот
	{
		std::cout << "Ошибка: прослушивания" << std::endl;
		close(*socket_fd);
		exit(EXIT_FAILURE);
	}
	for (;;)
	{
		struct sockaddr	*addr_client;
		socklen_t		addrlen = sizeof(*addr_client);
		int connect_fd = accept(*socket_fd, addr_client, &addrlen);
		if (connect_fd < 0)
		{
			std::cout << "Ошибка: принятия" << std::endl;
			close(*socket_fd);
			exit(EXIT_FAILURE);
		}
		fd_set fd;
		FD_ZERO(&fd); // снимаю все флаги в массиве файловых дескрипторов
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

		// Обычная процедура синтаксического анализа HTTP-сообщения заключается в считывании начальной строки в
		// структуру, считывании каждого поля заголовка в хеш-таблицу по имени поля до пустой строки, а затем с
		// использованием проанализированных данных, чтобы определить, ожидается ли тело сообщения. Если тело сообщения
		// было указано, то оно читается как поток, пока не будет прочитано количество октетов, равное длине тела
		// сообщения, или пока соединение не будет закрыто.
		if ((fd_read = FD_ISSET(*socket_fd, &fd)))
		{
			size_t ret = read(fd_read, (void*)message.input.c_str(), 2);
			while (ret && !message.input.find("\n\n"))
			{
				ret = read(fd_read, (void*)message.buf.c_str(), 1);
				message.input = message.input + message.buf;
				message.buf.clear();
			}
			parse_message(&message); // отправила Яне в парсер данные, считанные из сокета - в виде строке string
			interpret_message(&message); // в этой функции интерпретируем данные заголовка сообщения
			read_message_body(&message, fd_read); // в зависимости от анализа заголовка сообщения, происходит чтение
													// тела сообщения из потока
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
	if (bind(*socket_fd, (struct sockaddr*) addr, sizeof (*addr)) == -1) // снабжаем сокет адресом
	{
		std::cout << "Ошибка: связывания" << std:: endl;
		close(*socket_fd);
		exit(EXIT_FAILURE);
	}
}

void	fill_addr(sockaddr_in *addr, Config &config)
{
	(*addr).sin_family = PF_INET;
	(*addr).sin_port = htons(config.port); // преобразую в архитектуру сервера
	(*addr).sin_addr.s_addr = inet_addr(config.localhost); // заносит четырехбайтное целое число, отображающее корректный адрес, представленный в виде строки, в структуру. возвращает адрес в сетевом порядке байтов
//	(*addr).sin_addr.s_addr = htonl(INADDR_ANY); позволяет принимать соединения на заданный порт на любой ip, имеющийся в системе. вопрос - нужно ли нам это?
}

void	create_socket(int *socket_fd)
{
	*socket_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); // первый параметр - это семейство адресации - у нас TCP/IP, второй - тип взаимодействия (у нас потоковое, для дейтаграммного нужно SOCK_DGRAM) и указываем тип протокола - у нас TCP
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
	int					socket_fd; // слушающий сокет
	Config				config;

	check_count_arguments(argc);
	create_socket(&socket_fd);
	pars_config(argv[1], &config); // функция парсинга конфига сервера в структуру
	fill_addr(&addr, config);
	connect_to_socket(&addr, &socket_fd);
	work_with_socket(&addr, &socket_fd, &config);

	return (0);
}


/*
 * сетевой адрес сокета - это ip-адрес + номер порта
 * октет - это 8 бит
 * при передаче применяется big endian (старший байт передается первым)
 * htons, htonl, ntohl, ntohs используются для преобразования архитектур сети и машины (из little endian в big и наоборот)
 */