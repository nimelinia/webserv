//
// Created by Streetmentioner Copycat on 5/8/21.
//
#pragma once

#include <iostream>
#include <map>
#include "http/parsers/RequestParser.hpp"
#include "http/parsers/UriParser.hpp"

//#define BUFFER_SIZE	2100000
#define BUFFER_SIZE	1024000
//#define BUFFER_SIZE	1567415

namespace ft {
	class Message {
	public:
		Message();

		int							m_error_num;
		std::vector<http::Header>	m_headers;
		std::string 				m_body;
		std::string					m_method;
		std::string					m_uri_str;
		Uri							m_uri;
		std::string					host_name;
		int 						m_ver_major;
		int							m_ver_minor;
	};
}
