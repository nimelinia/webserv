//
// Created by Streetmentioner Copycat on 5/8/21.
//

#pragma once

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

namespace ft {
	struct Locations
	{
		std::list<std::string>				path_to_location;
		std::string							root;
		std::list<std::string>				allow;
		std::string							index;
		std::pair<size_t, std::string>		redirection;
		std::pair<std::string, std::string>	cgi;
		std::map<size_t, std::string>		error_pages;
		size_t								limit_body_size;
		bool 								autoindex;
		std::string							uploaded_folder;
	};


	struct Config
	{
		std::string							server_name;
		std::list<Locations>				locations;
		std::pair<size_t, std::string>		redirection;
		size_t								port;
		std::string 						hostaddress;
		std::map<size_t, std::string>		default_error_pages;
		std::string							default_root;
		std::string							default_index;
		bool 								default_autoindex;
		size_t								default_limit_body_size;
		std::list<std::string>				default_allow;

	};

	struct Host
	{
		std::list<Config>					configs;
	};
}

