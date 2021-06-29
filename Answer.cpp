//
// Created by Streetmentioner Copycat on 5/8/21.
//

#include "Answer.hpp"
#include "http/RequestParser.h"
#include "StatusCodes.hpp"


ft::Answer::Answer() :
	m_status_code(0),
//	m_server("WebServer of dream-team/1.0"),
//	m_date(Help::get_date()), 																								// по идее это лучше делать позже, в момент формирования ответа - сам объект создается в момент создания сокета
	m_body("")
{

}

ft::Answer::Answer(Config *config) :
		m_status_code(0),
//		m_server(config->server_name),
//		m_date(Help::get_date()), 																							// по идее это лучше делать позже, в момент формирования ответа - сам объект создается в момент создания сокета
		m_body("")
{

}


//
//void ft::Answer::make_error_answer(size_t num)
//{
//	m_status_code = num;
//	if (num == 400)
//		m_status_text = "Bad Request";
//	else if (num == 401)
//		m_status_text = "Unauthorized";
//	else if (num == 403)
//		m_status_text = "Forbidden";
//	else if (num == 404)
//	{
//		m_status_text = "Not Found";
//		std::ostringstream			oss;
//		std::ifstream				file;
//
//		file.open("./www/pages/404.html");
//		if (file.is_open()) {
//			oss << file.rdbuf();
//			m_body = oss.str();
//			m_body_exist = true;
//			file.close();
//		}
//	}
//	else if (num == 405)
//		m_status_text = "Method Not Allowed";
//	else if (num == 406)
//		m_status_text = "Not Acceptable";
//	else if (num == 408)
//		m_status_text = "Request Timeout";
//	else if (num == 413)
//		m_status_text = "Request Entity Too Large";
//	else if (num == 418)
//		m_status_text = "I'm a teapot";
//	else if (num == 500)
//		m_status_text = "Internal Server Error";
//	else if (num == 501)
//		m_status_text = "Not Implemented";
//	else if (num == 502)
//		m_status_text = "Bad Gateway";
//	else if (num == 503)
//		m_status_text = "Service Unavailable";
//	else if (num == 505)
//		m_status_text = "HTTP Version Not Supported";
//	m_connection = "close";
////	m_body = "";																											// нужно понять, откуда тело брать
//	m_content_length = m_body.length();
//	if (!m_body.empty())
//		m_content_type = "text/html; charset=iso-8859-1";
////	create_final_response();
//}


void ft::Answer::clean()
{

	m_body.clear();
	m_final_response.clear();
	m_status_code = 0;
	m_body_exist = false;
	m_size_response = 0;
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
		m_final_response += m_body;																							// в x.com я не видела переноса строки после тела ответа
//	std::cout << "Весь ответ: " << m_final_response << std::endl;															// это для теста, потом надо будет убрать
	m_size_response = m_final_response.length();
}








