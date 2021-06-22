//
// Created by Streetmentioner Copycat on 5/8/21.
//

#pragma once

//#ifndef WEBSERV_HPP
//#define WEBSERV_HPP

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
#include <sys/time.h>
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

//#define SERV_COUNT 100

struct Locations
{
	std::list<std::string>		path_to_location;
	std::string					root;
	std::string					allow;
	std::string					index;
	bool 						autoindex;
};


struct Config // нужно наполнить структуры необходимыми данными из конфиг-файла
{
	std::string				server_name;
	std::vector<Locations>	locations;
	size_t					port;
	char*					hostaddress;
	size_t					limit_body_size;

};

//#endif //WEBSERV_HPP
