cmake_minimum_required(VERSION 3.17)
project(webserv)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_executable(webserv
		main.cpp
		http/Client.cpp
		http/Message.cpp
		http/Message.hpp
		http/Answer.cpp
		server/Server.cpp
		server/Select.cpp
		server/AllServers.cpp
		http/parsers/RequestParser.cpp
		config/exceptions/ConfigException.cpp
		config/exceptions/ParseException.cpp
		config/exceptions/PathException.cpp
		config/exceptions/ValueException.cpp
		config/detail/Parser.cpp
		config/Section.cpp
		config/Config.cpp
		util/String.cpp
		util/File.cpp
		http/ResponseHandler.cpp
		http/ResponseHandler.hpp
		http/parsers/UriParser.cpp
		http/StatusCodes.cpp
		http/MimeTypes.cpp
		http/CgiHandler.cpp
		http/CgiProcess.cpp
		log/Level.cpp
		log/Formatters.cpp
		log/detail/LoggerBase.cpp
		log/detail/Message.cpp
		log/handlers/ColorConsoleHandler.cpp
		log/handlers/ConsoleHandler.cpp
		log/handlers/FileHandler.cpp
		config/config_analizator.hpp)