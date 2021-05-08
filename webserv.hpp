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
#include "Client.hpp"
#include "Message.hpp"
#include "Answer.hpp"

struct Config // нужно наполнить структуры необходимыми данными из конфиг-файла
{
	size_t		http;
	size_t		port;
	char 		*localhost;
	std::string	server_names;
	size_t		limit_body_size;

};

#endif //WEBSERV_HPP
