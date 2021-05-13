//
// Created by Streetmentioner Copycat on 5/8/21.
//

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <iostream>
#include <map>

class Message
{
public:
	std::string							input; // сюда записывается весь текст из прочтенного сообщения
	std::string							buf; // вспомогательая строка
	double								http_version;
	// тут нужны отдельно обязательные поля (типа хост, ip) и map по остальным полям
	std::string							method;
	std::map<std::string, std::string>	fields;
	bool								readable_body;
	std::string							message_body; // не факт, что должен быть такой тип данных
	bool								bad_request; // 400
	bool								wrong_version; // 505
	bool								redirection; // 301
	bool								long_url; // 414
	bool								non_realized; // 501
	bool								bad_gateway; // 502
};


#endif //MESSAGE_HPP
