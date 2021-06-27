//#include "webserv.hpp"
//#include "Client.hpp"
//#include "Server.hpp"
//#include "Message.hpp"
#include "AllServers.hpp"
#include "Help.hpp"
#include "config/Config.h"
#include "util/String.h"
#include "log/Log.h"

int	check_count_arguments(int argc)
{
	if (argc < 2)
	{
		std::cout << "There is no config for server!" << std::endl;
		return (1);
	}
	else if (argc > 2)
	{
		std::cout << "Too many arguments!" << std::endl;
	}
	return (0);
}

int main(int argc, char **argv) 																							// переписать, так как конфиг берем по пути, а не из аргументов
{
	::signal(SIGPIPE, SIG_IGN);

    LOGGER_(CGI).addHandler(new ft::log::ColorConsoleHandler(ft::log::TextOnlyFormatter));
    LOGGER_(CGI).setMaxLevel(ft::log::EDebug);

	if (check_count_arguments(argc))
		return (errno);																										// тут нужно прописать код ошибки
	ft::cfg::Config cfg;
	try {
		cfg.load(argv[1]);
	} catch (const ft::cfg::ConfigException & e)
	{
		std::cout << "Failed to load config: " << e.what() << std::endl;
		return 1;
	}

	ft::AllServers servers;
	std::list<ft::cfg::Section> sections = cfg.sectionList("server");
	for (std::list<ft::cfg::Section>::iterator it = sections.begin(); it != sections.end(); ++it) {
		Config	config;
		if (it->contains("server_name"))
			config.server_name = it->value("server_name");
		else
			config.server_name = "WebServer of dream-team/1.0";
		config.hostaddress = (char*)"127.0.0.1";
		std::list<std::string> listen_split = ft::util::str::Split(it->value("listen"), ':');
		if (listen_split.empty() || listen_split.size() > 2)
			std::cout << "error" << std::endl;
		config.port = std::strtoul(listen_split.back().c_str(), 0, 0);
		if (listen_split.size() == 2)
			config.hostaddress = listen_split.front();
		if (it->contains("client_max_body_size"))
			config.limit_body_size = std::strtoul(it->value("client_max_body_size").c_str(), 0, 0);
		else
			config.limit_body_size = 0;
		std::list<ft::cfg::Section> error_pages = it->sectionList("error_page");
		for (std::list<ft::cfg::Section>::iterator sit = error_pages.begin(); sit != error_pages.end(); ++sit)
		{
			const size_t status_code = std::strtoul(sit->value().c_str(), 0, 0);
			config.default_error_pages.insert(std::make_pair(status_code, sit->value(1)));
		}

		std::list<ft::cfg::Section> locations = it->sectionList("location");
		for(std::list<ft::cfg::Section>::iterator lit = locations.begin(); lit != locations.end(); ++lit)
		{
			config.locations.push_back(Locations());
			Locations& loc = config.locations.back();
			loc.path_to_location = lit->valueList();
			if (lit->contains("root"))
				loc.root = lit->value("root");
			if (lit->contains("limit_except"))
				loc.allow = lit->valueList("limit_except");
			if (lit->contains("index"))
				loc.index = lit->value("index");
			if (lit->contains("autoindex") && lit->value("autoindex") == "on")
				loc.autoindex = true;
			if (lit->contains("cgi"))
            {
			    loc.cgi.first = lit->value("cgi", 0);
			    loc.cgi.second = lit->value("cgi", 1);
            }
		}
		servers.create_server(config);
	}

	servers.start_all_servers();																							// нужно внутри сделать класс селект, который сделать синглтоном

	return (0);
}


/*
 * сетевой адрес сокета - это ip-адрес + номер порта
 * октет - это 8 бит
 * при передаче применяется big endian (старший байт передается первым)
 * htons, htonl, ntohl, ntohs используются для преобразования архитектур сети и машины (из little endian в big и наоборот)
 */
