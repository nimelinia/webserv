cmake_minimum_required(VERSION 3.17)
project(webserv)

set(CMAKE_CXX_STANDARD 98)

add_executable(webserv
		main.cpp
		webserv.hpp
		Client.cpp
		Client.hpp
		Message.cpp
		Message.hpp
		Answer.cpp
		Answer.hpp
		Server.cpp
		Server.hpp
		Select.cpp
		Select.hpp
		AllServers.cpp
		AllServers.hpp
		http/RequestParser.cpp
		http/RequestParser.h
		AllowedStd.h
		Help.cpp
		Help.hpp)