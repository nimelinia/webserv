//
// Created by Streetmentioner Copycat on 5/8/21.
//

#ifndef WEBSERV_ANSWER_HPP
#define WEBSERV_ANSWER_HPP

#include <iostream>

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

class Answer
{
	bool			body_exist;
	std::string		answer_body;
};


#endif //WEBSERV_ANSWER_HPP
