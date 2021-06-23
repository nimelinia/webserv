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
	};
	class UriParser
	{
	private:
		const std::list<Locations>	&m_root_list; // это location
	public:
		UriParser(const std::list<Locations>& root_list): m_root_list(root_list) {}
		Locations* m_location;
	public:
		bool parse_uri(const std::string& uri_str, Uri& uri);
	};

}


#endif //URIPARSER_HPP
