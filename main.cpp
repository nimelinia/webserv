//#include "webserv.hpp"
//#include "Client.hpp"
//#include "Server.hpp"
//#include "Message.hpp"
#include "AllServers.hpp"
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

bool find_root_location(std::list<ft::Locations>& location)
{
	for (std::list<ft::Locations>::iterator it = location.begin(); it != location.end(); ++it)
	{
		for (std::list<std::string>::iterator strit = it->path_to_location.begin(); strit != it->path_to_location.end(); ++strit)
		{
			if (*strit == "/")
				return (true);
		}
	}
	return false;
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
	if (!server.contains("listen"))
		throw std::runtime_error("there is no information about host/port");
	std::list<std::string> listen_split = ft::util::str::Split(server.value("listen"), ':');
	if (listen_split.empty() || listen_split.size() > 2)
//		std::cout << "error" << std::endl;
		throw std::runtime_error("config error with port and/or hostaddress");
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
	if (server.contains("root"))
		config.default_root = server.value("root");
	if (server.contains("index"))
		config.default_index = server.value("index");
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
	if (server.contains("autoindex") && server.value("autoindex") == "on")
		config.default_autoindex = true;
	if (server.contains("limit_except")) {
		config.default_allow = server.valueList("limit_except");
	}
	if (server.contains("limit_body_size"))
		config.default_limit_body_size = std::strtoul(server.value("limit_body_size").c_str(), 0, 0);
	else
		config.default_limit_body_size = LIMIT_BODY_SIZE;
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
		else if (!config.default_root.empty())
			loc.root = config.default_root;
		else
			throw std::runtime_error("No root directory in the config");													// если не может быть конфига без рута
		if (lit->contains("index"))
			loc.index = lit->value("index");
		else if (!config.default_index.empty())
			loc.index = config.default_index;
//		else
//			throw std::runtime_error("No default page in the config");														// нужно проверить, может ли быть конфиг без индекса
		std::list<ft::cfg::Section> error_pages_loc = lit->sectionList("error_page");
		for (std::list<ft::cfg::Section>::iterator eit = error_pages_loc.begin(); eit != error_pages_loc.end(); ++eit)
		{
			const size_t status_code = std::strtoul(eit->value().c_str(), 0, 0);
			loc.error_pages.insert(std::make_pair(status_code, eit->value(1)));
		}
		for (std::map<size_t, std::string>::iterator mit = config.default_error_pages.begin(); \
							mit != config.default_error_pages.end(); ++mit) 												// пробегаюсь по дефолтным, если не нахожу в location такого кода, то добавляю дефолтную
		{
			if (loc.error_pages.find(mit->first) == loc.error_pages.end())
				loc.error_pages.insert(*mit);
		}
		if (lit->contains("limit_except")) {
			loc.allow = lit->valueList("limit_except");
		} else if (!config.default_allow.empty())
			loc.allow = config.default_allow;
		if ((lit->contains("autoindex") && lit->value("autoindex") == "on") || config.default_autoindex)
			loc.autoindex = true;
		if (lit->contains("autoindex") && lit->value("autoindex") == "off")
			loc.autoindex = false;
		if (lit->contains("cgi"))
		{
			loc.cgi.first = lit->value("cgi", 0);
			loc.cgi.second = lit->value("cgi", 1);
		}
		if (lit->contains("limit_body_size"))
			loc.limit_body_size = std::strtoul(lit->value("limit_body_size").c_str(), 0, 0);
		else
			loc.limit_body_size = config.default_limit_body_size;
	}

	/*
	 * проверяю, есть ли у меня location, если их вообще нет, или нет корневого, то создаю с параметрами из директивы server
	 * если нет root и index и там, то кидаю ошибку
	 */

	if (config.locations.empty() || !find_root_location(config.locations))
	{
		if (config.default_root.empty() || config.default_index.empty())
			throw std::runtime_error("No root directory or/and default page in the config");
		config.locations.push_back(ft::Locations());
		ft::Locations &loc = config.locations.back();
		loc.root = config.default_root;
		loc.index = config.default_index;
		loc.error_pages = config.default_error_pages;
		loc.path_to_location.front() = "/";
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

//    LOGGER_(CGI).addHandler(new ft::log::ColorConsoleHandler(ft::log::TextOnlyFormatter));
//    LOGGER_(CGI).setMaxLevel(ft::log::EDebug);

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
