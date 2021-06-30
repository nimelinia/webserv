//
// Created by Streetmentioner Copycat on 5/8/21.
//
#pragma once
//#ifndef MESSAGE_HPP
//#define MESSAGE_HPP

#include <iostream>
#include <map>
#include "http/RequestParser.h"
#include "UriParser.hpp"

#define BUFFER_SIZE	1567415

namespace ft {
	class Message {
	public:
		Message();

//		void copy_buff(char *buf);

//		bool parse(char* buf);
//		bool read_body();

		void clean();

		void set_m_ready_response(bool ready_response);

//		bool						m_bad_request;																			// если в процессе парсинга станет ясно, что идет какая-то дичь, то тут надо ставить true
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

//#endif //MESSAGE_HPP
