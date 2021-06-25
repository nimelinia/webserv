cmake_minimum_required(VERSION 3.17)
project(webserv)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
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
		Help.hpp
		config/exceptions/ConfigException.cpp
		config/exceptions/ParseException.cpp
		config/exceptions/PathException.cpp
		config/exceptions/ValueException.cpp
		config/detail/Parser.cpp
		config/Section.cpp
		config/Config.cpp
		util/String.cpp
		util/File.cpp ResponseHandler.cpp ResponseHandler.hpp UriParser.cpp
		StatusCodes.cpp
		MimeTypes.cpp
		http/CgiHandler.cpp
		http/CgiProcess.cpp
		log/Level.cpp
		log/Formatters.cpp
		log/detail/LoggerBase.cpp
		log/detail/Message.cpp
		log/handlers/ColorConsoleHandler.cpp
		log/handlers/ConsoleHandler.cpp
		log/handlers/FileHandler.cpp
		)