//#include "webserv.hpp"
//#include "Client.hpp"
//#include "Server.hpp"
//#include "Message.hpp"
#include "AllServers.hpp"
#include "Help.hpp"
#include "config/Config.h"
#include "util/String.h"
#include "log/Log.h"

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

void fill_host(ft::cfg::Section server, std::list<ft::Host>& hosts)
{
	ft::Config	config;

	/*
	 * root, error_pages, index
	 * сначала кладем в сервер, потом подставляем в locations, если там нет своих
	 * если locations нет, генерим корневой location
	 */
/*
 * выясняем номер порта и хоста
 */
	std::list<std::string> listen_split = ft::util::str::Split(server.value("listen"), ':');
	if (listen_split.empty() || listen_split.size() > 2)
		std::cout << "error" << std::endl;
	config.port = std::strtoul(listen_split.back().c_str(), 0, 0);
	config.hostaddress = "0.0.0.0";
	if (listen_split.size() == 2)
		config.hostaddress = listen_split.front();
/*
 * вытаскиваем иные характеристики конфига, общие для всех locations
 */

	if (server.contains("server_name"))
		config.server_name = server.value("server_name");
	else
		config.server_name = config.hostaddress + ":" + ft::util::str::ToString(config.port);
	std::list<ft::cfg::Section> error_pages = server.sectionList("error_page");
	for (std::list<ft::cfg::Section>::iterator sit = error_pages.begin(); sit != error_pages.end(); ++sit)
	{
		const size_t status_code = std::strtoul(sit->value().c_str(), 0, 0);
		config.default_error_pages.insert(std::make_pair(status_code, sit->value(1)));
	}
	if (server.contains("redirect"))
	{
		std::list<std::string> redir = server.valueList("redirect");
		if (redir.size() != 2)
			throw std::runtime_error("config error with redirection");
		std::pair<size_t, bool> check = ft::util::str::FromString<size_t>(redir.front());
		if (!check.second)
			throw std::runtime_error("config error with redirection");
		config.redirection.first = check.first;
		config.redirection.second = redir.back();

	}
/*
 * заполняем данные для каждого location
 */
	std::list<ft::cfg::Section> locations = server.sectionList("location");
	for(std::list<ft::cfg::Section>::iterator lit = locations.begin(); lit != locations.end(); ++lit)
	{
		config.locations.push_back(ft::Locations());
		ft::Locations &loc = config.locations.back();
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
		if (lit->contains("limit_body_size"))
			loc.limit_body_size = std::strtoul(lit->value("limit_body_size").c_str(), 0, 0);
		else
			loc.limit_body_size = LIMIT_BODY_SIZE;
	}

	std::list<ft::Host>::iterator it;
	for (it = hosts.begin(); it != hosts.end(); ++it)
	{
		if (it->configs.front().port == config.port
			&& it->configs.front().hostaddress == config.hostaddress)
			break;
	}
	if (it != hosts.end() && it->configs.front().server_name == config.server_name)
	{
		throw std::runtime_error("There are some problem with the same virtual servers in your config!");
	}
	if (it == hosts.end())
		it = hosts.insert(hosts.end(), ft::Host());
	it->configs.push_back(config);
}


int main(int argc, char **argv)
{
	::signal(SIGPIPE, SIG_IGN);

    LOGGER_(CGI).addHandler(new ft::log::ColorConsoleHandler(ft::log::TextOnlyFormatter));
    LOGGER_(CGI).setMaxLevel(ft::log::EDebug);

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
