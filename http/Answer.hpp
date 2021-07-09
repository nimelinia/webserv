//
// Created by Streetmentioner Copycat on 5/8/21.
//

#pragma once

#include <iostream>
#include <dirent.h>
#include "http/Message.hpp"

namespace ft
{
	struct Answer
	{

		Answer();

		size_t					m_status_code;
		std::string				m_server;
		std::list<http::Header>	m_headers;
		bool					m_body_exist;
		std::string				m_body;
		std::string				m_final_response;

		std::string				m_path_to_file;

		void					clean();
		void					create_final_response();
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