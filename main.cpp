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

	Config		config;
	size_t i = 0;
	std::list<ft::cfg::Section> sections = cfg.sectionList("server");
	for (std::list<ft::cfg::Section>::iterator it = sections.begin(); it != sections.end(); ++it) {
		std::cout << "server_name: " << it->value("server_name") << " | "
				<< "listen: " << it->value("listen") << " | "
				<< it->section("location").value("root") << std::endl;
		config.server_name[i] = it->value("server_name");
		config.hostaddress[i] = (char*)"127.0.0.1";
		config.port[i] = std::strtoul(it->value("listen").c_str(), 0, 0);
		if (it->contains("client_max_body_size"))
			config.limit_body_size[i] = std::strtoul(it->value("client_max_body_size").c_str(), 0, 0);
		else
			config.limit_body_size[i] = 0;
		if (it->section("location").contains("root"))
			config.root[i] = it->section("location").value("root");
		if (it->section("location").contains("index"))
			config.index[i] = it->section("location").value("index");
		if (it->section("location").contains("limit_except"))
			config.allow[i] = it->section("location").value("limit_except");
		if (it->section("location").contains("autoindex") && it->section("location").value("autoindex") == "on")
			config.autoindex[i] = true;

		i++;
	}
	config.count_servers = i;
//	std::cout << ft::Help::get_date() << std::endl;


	ft::AllServers servers(config);
	servers.start_all_servers();																							// нужно внутри сделать класс селект, который сделать синглтоном

	return (0);
}


/*
 * сетевой адрес сокета - это ip-адрес + номер порта
 * октет - это 8 бит
 * при передаче применяется big endian (старший байт передается первым)
 * htons, htonl, ntohl, ntohs используются для преобразования архитектур сети и машины (из little endian в big и наоборот)
 */
