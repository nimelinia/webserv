//
// Created by Streetmentioner Copycat on 5/8/21.
//

#include "Answer.hpp"
#include "http/RequestParser.h"
#include "StatusCodes.hpp"
#include "webserv.hpp"


ft::Answer::Answer()
	: m_status_code(0)
	, m_body("")
{
}

void ft::Answer::clean()
{
	m_body.clear();
	m_final_response.clear();
	m_status_code = 0;
	m_body_exist = false;
	m_path_to_file.clear();
	m_headers.clear();
}

void ft::Answer::create_final_response()
{

	m_final_response = http::status_to_string(m_status_code);
	m_final_response += "Server: " + m_server + "\r\n";
	m_final_response += "Date: " + Help::get_date() + "\r\n";

	for (std::list<http::Header>::iterator it = m_headers.begin(); it != m_headers.end(); ++it)
		m_final_response += it->name + ": " + it->value + "\r\n";
	m_final_response += "\r\n";
	if (m_body_exist)
		m_final_response += m_body;
}








