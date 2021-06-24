//
// Created by Streetmentioner Copycat on 5/8/21.
//

#pragma once

#include <iostream>
#include <dirent.h>
#include "Message.hpp"
#include "Help.hpp"
#include "webserv.hpp"
//#include "Client.hpp"

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

		size_t					m_status_code;

		const std::string		m_server;
//		std::string				m_date;

		std::list<http::Header>	m_headers;

		bool					m_body_exist;
		std::string				m_body;

		std::string				m_final_response;
		size_t					m_size_response;


		std::string				m_path_to_file;

		void			clean();
		void			create_final_response();

	private:
		Answer();
	};
}

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