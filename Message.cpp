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
	pair = parser.parse(m_buff, m_read - m_parsed);
	m_parsed += pair.second;
	if (pair.first == http::RequestParser::EOk) 																			// если парсер закончил свою работу
	{
		m_headers = parser.m_headers;
		m_method = parser.m_method;
		m_uri = parser.m_uri;
		m_ver_major = parser.m_ver_major;
		m_ver_minor = parser.m_ver_minor;
		read_body();																										// что, если chunked
		m_ready_responce = true;
	}
	else if (pair.first == http::RequestParser::EParse)																		// если парсеру еще есть что читать
		return;
	else
		m_bad_request = true;
}

void ft::Message::clean()
{
	for (int i = 0; i < BUFFER_SIZE; ++i)
		m_buff[i] = '\0';
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
	{
		if (length + m_parsed > BUFFER_SIZE)
			length = BUFFER_SIZE - m_parsed;
		for (size_t i = 0; i < length; ++i)
			m_body[i] = m_buff[i + m_parsed];
	}

}
