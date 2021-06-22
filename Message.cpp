//
// Created by Streetmentioner Copycat on 5/8/21.
//

#include "Message.hpp"

ft::Message::Message() :
	m_bad_request(false),
	m_error_num(0),
	m_read(0),
	m_parsed(0),
	m_method(""),
	m_ver_major(0),
	m_ver_minor(0),
	m_ready_responce(false)
{

}

void ft::Message::copy_buff(char *buf)
{
	if (!buf)
		return;
	for (size_t i = 0; buf[i] != '\0'; ++i)
		m_buff[i] = buf[i];
}

bool ft::Message::parse(char *buf)
{
//	http::RequestParser									parser;
	std::pair<http::RequestParser::EResult, size_t>		pair;
	pair = m_parser.parse(buf, m_read - m_parsed);
	m_parsed += pair.second;
	if (pair.first == http::RequestParser::EOk) 																			// если парсер закончил свою работу
	{
		m_headers = m_parser.m_headers;
		m_method = m_parser.m_method;
		m_uri = m_parser.m_uri;
		m_ver_major = m_parser.m_ver_major;
		m_ver_minor = m_parser.m_ver_minor;
//		read_body();
		return (true);
		// что, если chunked
//		m_ready_responce = true;
	}
	else if (pair.first == http::RequestParser::EParse)																		// если парсеру еще есть что читать
		return (false);
	else
	{
		m_bad_request = true;
		m_error_num = 400;
//		m_ready_responce = true;
		return (false);
	}
//	m_ready_responce = true;
}

void ft::Message::clean()
{
	for (int i = 0; i < BUFFER_SIZE; ++i)
		m_buff[i] = '\0';
	m_bad_request = false;
	m_error_num = 0;
	m_read = 0;
	m_headers.clear();
	m_body.clear();
	m_method.clear();
	m_uri.clear();
	m_ver_minor = 0;
	m_ver_major = 0;
	m_parsed = 0,
	m_ready_responce = false;
	m_parser.reset();
}

bool ft::Message::read_body()
{
	size_t	length = 0;
	bool	readed = false;
	bool	finished = false;
	for (size_t i = 0; i < m_headers.size(); ++i)
	{
		if (m_headers[i].name == "Content-Length")
		{
			length = std::strtoul(m_headers[i].value.c_str(), 0, 0);
			readed = true;
			if (!length && m_headers[i].value != "0")
				readed = false;
			break;
		}
	}
	if (readed)
	{
		finished = true;
		if (length + m_parsed > BUFFER_SIZE)
		{
			length = BUFFER_SIZE - m_parsed;
			finished = false;
		}
		m_body.append(m_buff, m_parsed, length);
	} else
		finished = true;
	return (finished);
}

void ft::Message::setMReadyResponce(bool mReadyResponce)
{
	m_ready_responce = mReadyResponce;
}
