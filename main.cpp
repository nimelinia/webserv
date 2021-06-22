//#include "webserv.hpp"
//#include "Client.hpp"
//#include "Server.hpp"
//#include "Message.hpp"
#include "AllServers.hpp"
#include "Help.hpp"
#include "config/Config.h"

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

	std::vector<Config>		servers_conf;
	std::list<ft::cfg::Section> sections = cfg.sectionList("server");
	for (std::list<ft::cfg::Section>::iterator it = sections.begin(); it != sections.end(); ++it) {
		Config	config;
		std::cout << "server_name: " << it->value("server_name") << " | "
				<< "listen: " << it->value("listen") << " | "
				<< it->section("location").value("root") << std::endl;
		config.server_name = it->value("server_name");
		config.hostaddress = (char*)"127.0.0.1";
		config.port = std::strtoul(it->value("listen").c_str(), 0, 0);
		if (it->contains("client_max_body_size"))
			config.limit_body_size = std::strtoul(it->value("client_max_body_size").c_str(), 0, 0);
		else
			config.limit_body_size = 0;
		std::list<ft::cfg::Section> locations = it->sectionList("location");
//		std::list<std::string> value = it->valueList("location");
		for(std::list<ft::cfg::Section>::iterator it = locations.begin(); it != locations.end(); ++it)
		{
			Locations	loc;
			loc.path_to_location = it->valueList();
			if (it->contains("root"))
				loc.root = it->value("root");
			if (it->contains("limit_except"))
				loc.allow = it->value("limit_except");
			if (it->contains("index"))
				loc.index = it->value("index");
			if (it->contains("autoindex") && it->value("autoindex") == "on")
				loc.autoindex = true;
			config.locations.push_back(loc);
		}
		servers_conf.push_back(config);
	}
//	config.count_servers = i;
//	std::cout << ft::Help::get_date() << std::endl;


	ft::AllServers servers(servers_conf);
	servers.start_all_servers();																							// нужно внутри сделать класс селект, который сделать синглтоном

	return (0);
}


/*
 * сетевой адрес сокета - это ip-адрес + номер порта
 * октет - это 8 бит
 * при передаче применяется big endian (старший байт передается первым)
 * htons, htonl, ntohl, ntohs используются для преобразования архитектур сети и машины (из little endian в big и наоборот)
 */
