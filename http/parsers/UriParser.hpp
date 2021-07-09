//
// Created by Streetmentioner Copycat on 6/23/21.
//

#ifndef URIPARSER_HPP
#define URIPARSER_HPP

#include <iostream>
#include "util/String.hpp"
#include "server/webserv.hpp"


struct Config;
struct Locations;


namespace ft {
	struct Uri
	{
		std::string root;
		std::string path;
		std::string file_name;
		std::string file_ext;
		std::string extra_path;
		std::string query;
		const Locations*	locations;
		Config*		config;
		void clean()
		{
			root.clear();
			path.clear();
			file_name.clear();
			file_ext.clear();
			extra_path.clear();
			query.clear();
			locations = NULL;
			config = NULL;
		}
	};

	struct FindLocation: public std::unary_function<Locations, bool>
	{
		std::string m_name;
		FindLocation(const std::string& name): m_name(name) {}
		bool operator()(const Locations& loc)
		{
			for (std::list<std::string>::const_iterator cit = loc.path_to_location.begin();
					cit != loc.path_to_location.end(); ++cit)
			{
				if(*cit == m_name)
					return true;
			}
			return false;
		}
	};

	class UriParser
	{
	private:
		const std::list<Locations>&	m_root_list;
		std::string 		m_method;
		bool				find_path_of_uri(std::string &cur_path, const std::string &path, Uri& uri);
	public:
		UriParser(const std::list<Locations>& root_list, std::string& method)
		    : m_root_list(root_list), m_method(method), m_location(NULL) {}
		const Locations* 					m_location;
	public:
		bool parse_uri(const std::string& uri_str, Uri& uri);
	};

}


#endif //URIPARSER_HPP
