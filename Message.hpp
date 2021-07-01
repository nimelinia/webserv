//
// Created by Streetmentioner Copycat on 5/8/21.
//
#pragma once

#include <iostream>
#include <map>
#include "http/RequestParser.h"
#include "UriParser.hpp"

#define BUFFER_SIZE	1567415

namespace ft {
	class Message {
	public:
		Message();

		void clean();

		void set_m_ready_response(bool ready_response);

		int							m_error_num;																			// код ошибки, которую отправить клиенту
		std::vector<http::Header>	m_headers;
		std::string 				m_body;
		std::string					m_method;
		std::string					m_uri_str;
		Uri							m_uri;
		std::string					host_name;
		int 						m_ver_major;
		int							m_ver_minor;
		bool 						m_ready_responce;
	};
}
