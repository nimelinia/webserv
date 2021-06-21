//
// Created by Streetmentioner Copycat on 5/8/21.
//

#pragma once
//#ifndef WEBSERV_ANSWER_HPP
//#define WEBSERV_ANSWER_HPP

#include <iostream>
#include <dirent.h>
//#include "Message.hpp"
#include "Help.hpp"
#include "webserv.hpp"
#include "Client.hpp"

#define BEFORE_BODY "<html lang=\"en\"><head><meta charset=\"utf-8\"><title>Содержимое</title><style>body{margin: 40px; margin-left: 60px;color: #ff001e;font-size: 26px;font-family: \"PT Serif\",Georgia,Times New Roman,Times,serif;font-style: normal;}</style></head><body>"
#define AFTER_BODY "</body ></html >"

/*
 * Наличие тела сообщения в ответе зависит как от метода запроса, на который он отвечает, так и от кода состояния
 * ответа (раздел 3.1.2). Ответы на метод запроса HEAD (раздел 4.3.2 [RFC7231 #]) никогда не включают в себя тело
 * сообщения, поскольку соответствующие поля заголовка ответа (например, Transfer-Encoding, Content-Length и т. д.),
 * если присутствует, указывает только то, что их значения были бы, если бы метод запроса был GET (Раздел 4.3.1
 * [RFC7231 #]). 2xx (успешные) ответы на метод запроса CONNECT (раздел 4.3.6 [RFC7231 #]) переключаются в туннельный
 * режим вместо того, чтобы иметь тело сообщения. Все ответы 1xx (информационный), 204 (без содержимого) и 304
 * (без изменений) не содержат тела сообщения. Все остальные ответы включают тело сообщения,
 * хотя тело может иметь нулевую длину.
 */

namespace ft
{
	class Answer
	{
	public:

		explicit Answer(Config *config);

		std::string		m_protocol_v;
		size_t			m_status_code;
		std::string		m_status_text;

		std::string		m_server;
		std::string		m_location;
		std::string 	m_connection;
		std::string		m_retry_after;
		std::string		m_allow;
		std::string		m_content_type;
		size_t			m_content_length;
		bool			m_length_exist;
		std::string		m_content_language;
		std::string		m_content_location;																					// не понятно, нужно ли это
		std::string		m_date;

		std::string		m_last_modified;
		std::string 	m_transfer_encoding;


		bool			m_body_exist;
		std::string		m_body;

		std::string		m_final_response;
		size_t			m_size_response;
		std::string		m_path_to_file;

		Config			*m_config;
		std::string		m_uri;
		Locations		*m_conf_location;


		void			check_validity(Message &message);
		void			check_allow_methods(Message &message);
		void			generate_answer(Message &message);
		void			find_path_to_file(Message &message);
		void			make_error_answer(size_t num);
		void			clean();
		void			create_final_response();
		void			create_response_body();
		std::string		detect_content_type();
		std::string 	detect_last_modified();

	private:
		std::string		m_slash;
		void			generate_GET();
		void 			generate_POST();
		void			generate_DELETE();
		void			wrong_method();
		std::string		cut_part_path(std::string &path);
		Answer();
	};
}



//#endif //WEBSERV_ANSWER_HPP

/*
 * 200	OK
 * 201	Успешная команда POST
 * 202	Запрос принят
 * 203	Запрос GET или HEAD выполнен
 * 204	Запрос выполнен но нет содержимого
 * 300	Ресурс обнаружен в нескольких местах
 * 301	Ресурс удален навсегда
 * 302	Ресурс отсутствует временно
 * 304	Ресурс был изменен
 * 400	Плохой запрос от клиента
 * 401	Неавторизованый запрос
 * 402	Необходима оплата за ресурс
 * 403	Доступ Запрещен
 * 404	Ресурс не найден
 * 405	Метод не применим для данного ресурса
 * 406	Недопустимый тип ресурса
 * 410	Ресурс Недоступен
 * 500	Внутренняя ошибка сервера (это по вашу душу,юные CGI-программисты ;( )
 * 501	Метод не выполнен
 * 502	Неисправный шлюз либо перегруз сервера
 * 503	Сервер недоступен/тайм-аут шлюза
 * 504	Вторичный шлюз/тай-аут сервера
 */