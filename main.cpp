//#include "webserv.hpp"
//#include "Client.hpp"
//#include "Server.hpp"
//#include "Message.hpp"
#include "server/AllServers.hpp"
#include "config/Config.hpp"
#include "util/String.hpp"
#include "log/Log.hpp"
#include "config/config_analizator.hpp"

#define LIMIT_BODY_SIZE 10240

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

int main(int argc, char **argv)
{
	::signal(SIGPIPE, SIG_IGN);

    LOGGER_(CGI).addHandler(new ft::log::ColorConsoleHandler(ft::log::TextOnlyFormatter));
    LOGGER_(CGI).setMaxLevel(ft::log::EDebug);

//    ft::log::FileHandler* handler = new ft::log::FileHandler(ft::log::TextOnlyFormatter);
//    handler->open("webserv.log");
//    LOGGER.addHandler(handler);
    LOGGER.addHandler(new ft::log::ColorConsoleHandler(ft::log::TextOnlyFormatter));
	LOGGER.setMaxLevel(ft::log::EDebug);

	if (check_count_arguments(argc))
		return (errno);

	ft::cfg::Config cfg;
	std::list<ft::Host> hosts;
	try {
		cfg.load(argv[1]);
		std::list<ft::cfg::Section> sections = cfg.sectionList("server");
		for (std::list<ft::cfg::Section>::iterator it = sections.begin(); it != sections.end(); ++it)
			fill_host(*it, hosts);
	} catch (const ft::cfg::ConfigException & e)
	{
		std::cout << "Failed to load config: " << e.what() << std::endl;
		return 1;
	} catch (const std::exception& e)
	{
		std::cout << "Error: " << e.what() << std::endl;
		return 1;
	}

	ft::AllServers servers;
	for (std::list<ft::Host>::iterator hit = hosts.begin(); hit != hosts.end(); ++hit)
		servers.create_server(*hit);
	servers.start_all_servers();
	return (0);
}


/*
 * сетевой адрес сокета - это ip-адрес + номер порта
 * октет - это 8 бит
 * при передаче применяется big endian (старший байт передается первым)
 * htons, htonl, ntohl, ntohs используются для преобразования архитектур сети и машины (из little endian в big и наоборот)
 */
