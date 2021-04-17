#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <queue>
#include <stack>
#include <map>
#include <algorithm>
#include <exception>
#include <ctime>
#include <cstring>
#include <sys/socket.h>
#include <fcntl.h>
#include <cstdlib>
#include <unistd.h>
#include <csignal>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <cstdio>
#include <cerrno>
#include <ctime>
#include <sys/select.h>
#include <sys/types.h>


int main(int argc, char **argv)
{
//	if (argc != 2)
//	{
//		std::cout << "There is no config for server!" << std::endl;
//		return (1);
//	}
	struct sockaddr_in addr;
	int socket_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (socket_fd == -1)
	{
		std::cout << "ошибка при создании сокета" << std::endl;
		exit(EXIT_FAILURE);
	}

	addr.sin_family = PF_INET;
	addr.sin_port = htons(8080);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (bind(socket_fd, (struct sockaddr*) &addr, sizeof (addr)) == -1)
	{
		std::cout << "Ошибка: связывания" << std:: endl;
		close(socket_fd);
		exit(EXIT_FAILURE);
	}

	if (listen(socket_fd, -1) == -1)
	{
		std::cout << "Ошибка: прослушивания" << std::endl;
		close(socket_fd);
		exit(EXIT_FAILURE);
	}

//	for (;;)
//	{
		int connect_fd = accept(socket_fd, 0, 0);

		if (connect_fd < 0)
		{
			std::cout << "Ошибка: принятия" << std::endl;
			close(socket_fd);
			exit(EXIT_FAILURE);
		}
		write(connect_fd, "hello from server\n", 19);
		close(connect_fd);
//	}
	return (0);
}
