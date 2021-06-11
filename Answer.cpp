//
// Created by Streetmentioner Copycat on 5/8/21.
//

#include "Answer.hpp"

ft::Answer::Answer() :
	m_protocol_v("HTTP/1.1"),
	m_location(""),
	m_connection(""),
	m_retry_after(""),
	m_allow(""),
	m_server("WebServer of dream-team/1.0"),
	m_content_type(""),
	m_content_length(0),
	m_length_exist(false),
	m_content_location(""),
	m_date(Help::get_date()), // по идее это лучше делать позже, в момент формирования ответа - сам объект создается в момент создания сокета
	m_last_modified(""),
	m_transfer_encoding(""),
	m_body("")
{

}

void ft::Answer::generate_answer(ft::Message &message)
{
	if (message.m_error_num == 413)																							// в стандарте указано, что если неспособность обработать запрос - временная, нужно отправить retry_after
	{
		m_status_code = 413;
		m_status_text = "Request Entity Too Large";
		m_content_length = 449;
		m_connection = "close";
		m_content_type = "text/html; charset=iso-8859-1";
		create_final_response();
		return;
	}
	if (message.m_method == "GET")
		generate_GET();
	else if (message.m_method == "POST")
		generate_POST();
	else if (message.m_method == "DELETE")
		generate_DELETE();
	else
		wrong_method();
	create_final_response();
}

void ft::Answer::clean()
{

}

void ft::Answer::generate_GET()
{
	m_status_code = 200;
	m_status_text = "Ok";
}

void ft::Answer::generate_POST()
{
	m_status_code = 201;
	m_status_text = "Created";
}

void ft::Answer::generate_DELETE()
{
	m_status_code = 200;
	m_status_text = "Ok";

	m_status_code = 202;
	m_status_text = "Accepted";

	m_status_code = 204;
	m_status_text = "No Content";
}

void ft::Answer::wrong_method()
{
	m_status_code = 501;
	m_status_text = "Not Implemented";
}

void ft::Answer::create_final_response()
{
	m_final_response += m_protocol_v + " " + Help::to_string(m_status_code) + " " + m_status_text + "\r\n";
	m_final_response += "Server: " + m_server + "\r\n";
	m_final_response += "Date: " + Help::get_date() + "\r\n";
	if (!m_connection.empty())
		m_final_response += "Connection: " + m_connection + "\r\n";
	if (!m_location.empty())
		m_final_response += "Location: " + m_location + "\r\n";
	if (!m_retry_after.empty())
		m_final_response += "Retry-After: " + m_retry_after + "\r\n";
	if (!m_last_modified.empty())
		m_final_response += "Last-Modified: " + m_last_modified + "\r\n";
	if (!m_allow.empty())
		m_final_response += "Allow: " + m_allow + "\r\n";
	if (!m_content_type.empty())
		m_final_response += "Content-Type: " + m_content_type + "\r\n";
	if (m_length_exist)
		m_final_response += "Content-Length: " + Help::to_string(m_content_length) + "\r\n";
	if (!m_transfer_encoding.empty())
		m_final_response += "Transfer-Encoding: " + m_transfer_encoding + "\r\n";
	if (!m_content_language.empty())
		m_final_response += "Content-Language: " + m_content_language + "\r\n";
	if (!m_content_location.empty())
		m_final_response += "Content-Location: " + m_content_location + "\r\n";
	m_final_response += "\r\n";
	if (m_body_exist)
		m_final_response += m_body;																							// в x.com я не видела переноса строки после тела ответа
}
