//
// Created by Streetmentioner Copycat on 5/8/21.
//

#include "Message.hpp"

ft::Message::Message() :
//	m_bad_request(false),
	m_error_num(0),
	m_method(""),
	m_ver_major(0),
	m_ver_minor(0),
	m_ready_responce(false)
{

}

void ft::Message::clean()
{
//	m_bad_request = false;
	m_error_num = 0;
	m_headers.clear();
	m_body.clear();
	m_method.clear();
	m_uri.clear();
	m_ver_minor = 0;
	m_ver_major = 0;
	m_ready_responce = false;
}

void ft::Message::set_m_ready_response(bool ready_response)
{
	m_ready_responce = ready_response;
}
