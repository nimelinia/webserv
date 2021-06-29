//
// Created by Streetmentioner Copycat on 6/23/21.
//

#ifndef URIPARSER_HPP
#define URIPARSER_HPP

#include <iostream>
#include "util/String.h"
#include "webserv.hpp"


namespace ft {
	struct Uri
	{
		std::string root;
		std::string path;
		std::string file_name;
		std::string file_ext;
		std::string extra_path;
		std::string query;
//		std::string non_exist_path;
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
		const std::list<Locations>&	m_root_list; // это location
		std::string 		m_method;
		bool				find_path_of_uri(std::string &cur_path, const std::string &path, Uri& uri);
	public:
		UriParser(const std::list<Locations>& root_list, std::string& method): m_root_list(root_list), m_location(NULL), m_method(method) {}
		const Locations* 					m_location;
	public:
		bool parse_uri(const std::string& uri_str, Uri& uri);
	};

}


#endif //URIPARSER_HPP
