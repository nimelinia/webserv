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
	m_body(""),
	m_slash("")
{

}

ft::Answer::Answer(Config *config) :
		m_protocol_v("HTTP/1.1"),
//		m_server("WebServer of dream-team/1.0"),
		m_server(config->server_name),
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
		m_body(""),
		m_config(config),
		m_slash("")
{

}

void ft::Answer::check_validity(ft::Message &message)
{

	if (message.m_method.empty() || message.m_ver_major < 1 || message.m_bad_request)
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
	if (message.m_uri == "path from config for method GET")																	// это какая то чушь написана
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
	{
		m_status_text = "Not Found";
		std::ostringstream			oss;
		std::ifstream				file;

		file.open("./www/pages/404.html");
		if (file.is_open()) {
			oss << file.rdbuf();
			m_body = oss.str();
			m_body_exist = true;
			file.close();
		}
	}
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
//	m_body = "";																											// нужно понять, откуда тело брать
	m_content_length = m_body.length();
	if (!m_body.empty())
		m_content_type = "text/html; charset=iso-8859-1";
//	create_final_response();
}


std::string ft::Answer::cut_part_path(std::string &path)
{
	int pos;
	std::string part_path;

	pos = path.find_last_of('/');
	if (pos == std::string::npos)
		return ("");
//	if (pos != 0)
//		pos -= 1;
	if (pos == 0)
		pos = 1;
	part_path = path.substr(0, pos);
	return (part_path);
}



void ft::Answer::find_path_to_file(ft::Message &message)
{
	std::string		path_buf;

	if (message.m_uri[message.m_uri.length() - 1] != '/') {
		m_slash = "/";
	}
	path_buf = message.m_uri + m_slash;
//	if (!m_slash.empty())
//		path_buf = cut_part_path(path_buf);
	while (!path_buf.empty())
	{
		for (size_t i = 0; i < m_config->locations.size(); ++i) {
			for (std::list<std::string>::iterator it = m_config->locations[i].path_to_location.begin();
				it != m_config->locations[i].path_to_location.end(); ++it) {
				if (path_buf == *it) {
					m_conf_location = &m_config->locations[i];
					m_path_to_file = m_conf_location->root + m_uri;
					return;
				}
			}
		}
		if (path_buf == "/")
			return;
		path_buf = cut_part_path(path_buf);
	}
}

void ft::Answer::generate_answer(ft::Message &message)
{
	m_uri = message.m_uri;
	if (!message.m_error_num)
		check_validity(message);
	if (message.m_error_num)																							// в стандарте указано, что если неспособность обработать запрос - временная, нужно отправить retry_after
	{
		make_error_answer(message.m_error_num);
		return;
	}
	find_path_to_file(message);
//	m_path_to_file = DOT + m_config->root[message.m_client_id] + message.m_uri;
//	if (message.m_uri == "/")
//		m_path_to_file += m_config->index[message.m_client_id];
	if (message.m_method == "GET" && !m_status_code)
		generate_GET();
	else if (message.m_method == "POST" && !m_status_code)
		generate_POST();
//		generate_GET();
	else if (message.m_method == "DELETE" && !m_status_code)
		generate_DELETE();
	else if (!m_status_code)
		wrong_method();
	create_final_response();
}

void ft::Answer::clean()
{

	m_status_text.clear();
	m_location.clear();
	m_connection.clear();
	m_retry_after.clear();
	m_allow.clear();
	m_content_type.clear();
	m_content_language.clear();
	m_content_location.clear();
	m_last_modified.clear();
	m_transfer_encoding.clear();
	m_body.clear();
	m_final_response.clear();
	m_protocol_v = "HTTP/1.1";
	m_status_code = 0;
	m_server = m_config->server_name;
//	m_server = "WebServer of dream-team/1.0";
	m_content_length = 0;
	m_length_exist = false;
	m_body_exist = false;
	m_size_response = 0;
	m_uri.clear();
	m_path_to_file.clear();
}

void ft::Answer::create_response_body()
{
	std::ostringstream			oss;
	std::ifstream				file;
	DIR							*dir;
	struct dirent				*ent;
	std::string					slash;
	struct stat buff;
	stat(m_path_to_file.c_str(), &buff);

	if (m_uri[m_uri.length() - 1] != '/')
		slash = "/";
	if ((buff.st_mode & S_IFMT) != S_IFDIR)
	{
		file.open(m_path_to_file);
		if (file.is_open()) {
			oss << file.rdbuf();
			m_body = oss.str();
			m_body_exist = true;
			file.close();
			return;
		} else {
			make_error_answer(404);
			/* could not open directory */
			perror ("");
			return;
		}
	}
	else
	{
		if (!m_conf_location->index.empty())
		{
			stat((m_path_to_file + slash + m_conf_location->index).c_str(), &buff);
			if ((buff.st_mode & S_IFMT) != S_IFDIR) {
				file.open(m_path_to_file + slash + m_conf_location->index);
				if (file.is_open()) {
					oss << file.rdbuf();
					m_body = oss.str();
					m_body_exist = true;
					file.close();
					return;
				}
			}
		}
		if (m_conf_location->autoindex == true)
		{
			if ((dir = opendir(m_path_to_file.c_str())) != NULL)
			{
				oss << BEFORE_BODY;
				while ((ent = readdir(dir)) != NULL)
					oss << "<a href='" << m_uri + slash + ent->d_name << "'>" << ent->d_name << "</a><br />";
				closedir (dir);
				oss << AFTER_BODY;
				m_body = oss.str();
				m_body_exist = true;
			}
			else
			{
				make_error_answer(404);
				/* could not open directory */
				perror ("");
				return;
			}
		}
		else
		{
			make_error_answer(404);
			/* could not open directory */
			perror ("");
			return;
		}
	}
}

void ft::Answer::generate_GET()
{
	m_status_code = 200;
	m_status_text = "Ok";
	m_connection = "Upgrade";
	create_response_body();
	if (m_body_exist)
	{
		m_content_length = m_body.length();
		m_length_exist = true;
	}
	if (m_status_code == 200)
	{
		m_content_type = detect_content_type();
		m_content_location = m_path_to_file;
		m_last_modified = detect_last_modified();
	}
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

std::string ft::Answer::detect_content_type()
{
	return ("");
}

std::string ft::Answer::detect_last_modified()
{
	struct stat buff;
	std::string	file;
	file = m_path_to_file;
	stat(file.c_str(), &buff);
	return (Help::get_date(buff.st_ctimespec));
}






