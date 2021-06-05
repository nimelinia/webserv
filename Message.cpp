//
// Created by Streetmentioner Copycat on 5/8/21.
//

#include "Message.hpp"

ft::Message::Message()
{

}

void ft::Message::copy_buff(char *buf)
{
	if (!buf)
		return;
	for (size_t i = 0; buf[i] != '\0'; ++i)
		m_buff[i] = buf[i];
}

void ft::Message::parse()
{
	http::RequestParser									parser;
	std::pair<http::RequestParser::EResult, size_t>		pair;
	pair = parser.parse(m_buff, m_readed - m_parsed);
	m_parsed += pair.second;
	if (pair.second == http::RequestParser::EOk) 																			// если парсер закончил свою работу
	{
		m_headers = parser.m_headers;
		read_body();
	}
	else if (pair.second == http::RequestParser::EParse)																	// если парсеру еще есть что читать
	{

	}
	else
		m_bad_request = true;
}

void ft::Message::clean()
{
	m_buff[0] = '\0';
}

void ft::Message::read_body()
{
	size_t	length = -1;
	for (size_t i = 0; i < m_headers.size(); ++i)
	{
		if (m_headers[i].name == "Content-Length")
		{
			length = std::strtol(m_headers[i].value.c_str(), 0, 0);
			break;
		}
	}
	if (length != -1)
		// тут надо перенести из уже прочитанного, но не являющегося заголовком, того, что в буфере, в body

}
