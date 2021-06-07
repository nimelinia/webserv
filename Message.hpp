//
// Created by Streetmentioner Copycat on 5/8/21.
//

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <iostream>
#include <map>
#include "http/RequestParser.h"

#define BUFFER_SIZE	1567415

namespace ft {
	class Message {
	public:
		Message();

		void copy_buff(char *buf);

		void parse();
		void read_body();

		void clean();

		char						m_buff[BUFFER_SIZE];
		bool						m_bad_request;																			// если в процессе парсинга станет ясно, что идет какая-то дичь, то тут надо ставить true
		int							m_error_num;																			// код ошибки, которую отправить клиенту
		size_t						m_readed;
		size_t						m_parsed;
		std::vector<http::Header>	m_headers;
		char 						m_body[BUFFER_SIZE];
		std::string					m_method;
		std::string					m_uri;
		int 						m_ver_major;
		int							m_ver_minor;
		bool 						m_ready_responce;
//	std::string							input; // сюда записывается весь текст из прочтенного сообщения
//	std::string							buf; // вспомогательая строка
//	double								http_version;
//	// тут нужны отдельно обязательные поля (типа хост, ip) и map по остальным полям
//	std::string							method;
//	std::map<std::string, std::string>	fields;
//	bool								readable_body;
//	std::string							message_body; // не факт, что должен быть такой тип данных
//	bool								bad_request; // 400
//	bool								wrong_version; // 505
//	bool								redirection; // 301
//	bool								long_url; // 414
//	bool								non_realized; // 501
//	bool								bad_gateway; // 502
	};
}

#endif //MESSAGE_HPP
