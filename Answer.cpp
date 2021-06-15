//
// Created by Streetmentioner Copycat on 5/8/21.
//

#include "Answer.hpp"

ft::Answer::Answer() :
	m_protocol_v("HTTP/1.1"),
	m_server("WebServer of dream-team/1.0"),
	m_location(""),
	m_connection(""),
	m_retry_after(""),
	m_allow(""),
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

void ft::Answer::check_validity(ft::Message &message)
{
	if (message.m_method.empty() || message.m_ver_major < 1)
		message.m_error_num = 400;
	else if (message.m_method != "GET" && message.m_method != "POST" && message.m_method != "DELETE")
	{
		message.m_error_num = 501;
		check_allow_methods(message);																					// нужно написать метод, который проверяет методы, которые доступны с такими параметрами
	}
//	else if (check_this_method(message.m_method, message))
//		message.m_error_num = 405;

}

void ft::Answer::check_allow_methods(Message &message)																		// нужно как-то понять, где и как проверять доступность методов
{
	if (message.m_uri == "path from config for method GET")
		m_allow += "GET";
	if (message.m_uri == "path from config for method POST") {
		if (!m_allow.empty())
			m_allow += ", ";
		m_allow += "POST";
	}
	if (message.m_uri == "path from config for method DELETE")
	{
		if (!m_allow.empty())
			m_allow += ", ";
		m_allow += "POST";
	}
}


void ft::Answer::make_error_answer(size_t num)
{
	m_status_code = num;
	if (num == 400)
		m_status_text = "Bad Request";
	else if (num == 401)
		m_status_text = "Unauthorized";
	else if (num == 403)
		m_status_text = "Forbidden";
	else if (num == 404)
		m_status_text = "Not Found";
	else if (num == 405)
		m_status_text = "Method Not Allowed";
	else if (num == 406)
		m_status_text = "Not Acceptable";
	else if (num == 408)
		m_status_text = "Request Timeout";
	else if (num == 413)
		m_status_text = "Request Entity Too Large";
	else if (num == 418)
		m_status_text = "I'm a teapot";
	else if (num == 500)
		m_status_text = "Internal Server Error";
	else if (num == 501)
		m_status_text = "Not Implemented";
	else if (num == 502)
		m_status_text = "Bad Gateway";
	else if (num == 503)
		m_status_text = "Service Unavailable";
	else if (num == 505)
		m_status_text = "HTTP Version Not Supported";
	m_connection = "close";
	m_body = "";																											// нужно понять, откуда тело брать
	m_content_length = m_body.length();
	if (!m_body.empty())
		m_content_type = "text/html; charset=iso-8859-1";
	create_final_response();
}

void ft::Answer::generate_answer(ft::Message &message)
{
	if (!message.m_error_num)
		check_validity(message);
	if (message.m_error_num)																							// в стандарте указано, что если неспособность обработать запрос - временная, нужно отправить retry_after
	{
		make_error_answer(message.m_error_num);
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
	m_protocol_v = "HTTP/1.1";
	m_status_code = 0;
	m_status_text.clear();
	m_location.clear();
	m_connection.clear();
	m_retry_after.clear();
	m_allow.clear();
	m_server = "WebServer of dream-team/1.0";
	m_content_type.clear();
	m_content_length = 0;
	m_length_exist = false;
	m_content_language.clear();
	m_content_location.clear();
	m_date = Help::get_date();
	m_last_modified.clear();
	m_transfer_encoding.clear();
	m_body_exist = false;
	m_body.clear();
	m_final_response.clear();
	m_size_response = 0;
}

void ft::Answer::generate_GET()
{
	m_status_code = 200;
	m_status_text = "Ok";
	m_connection = "Upgrade";
}

void ft::Answer::generate_POST()
{
	m_status_code = 200;
	m_status_text = "Ok";

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
	std::cout << "Весь ответ: " << m_final_response << std::endl;															// это для теста, потом надо будет убрать
	m_size_response = m_final_response.length();
}




