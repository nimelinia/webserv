//
// Created by Streetmentioner Copycat on 5/8/21.
//

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

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
//#include "Client.hpp"
//#include "Message.hpp"
//#include "Answer.hpp"
//#include "Select.hpp"
//#include "Server.hpp"
//#include "AllServers.hpp"

#define SERV_COUNT 1000

struct Config // нужно наполнить структуры необходимыми данными из конфиг-файла
{
	size_t		count_servers;
	std::string	server_name[SERV_COUNT];
	std::string root[SERV_COUNT];
	std::string allow[SERV_COUNT];
	std::string index[SERV_COUNT];
	bool 		autoindex[SERV_COUNT];
//	size_t		http;
	size_t		port[SERV_COUNT];
	char 		*hostaddress[SERV_COUNT];
	size_t		limit_body_size[SERV_COUNT];

};

#endif //WEBSERV_HPP
