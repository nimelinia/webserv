//
// Created by Streetmentioner Copycat on 6/23/21.
//

#include "ResponseHandler.hpp"
#include "Client.hpp"
#include "MimeTypes.hpp"
#include "StatusCodes.hpp"

ft::ResponseHandler::ResponseHandler(Config &config, Client& client) :
	m_config(config),
	m_client(client),
	m_location(NULL)
{

}

bool ft::ResponseHandler::generate_answer()
{
    Message& m_msg = m_client.m_msg;
    Answer& m_answer = m_client.m_answer;

	UriParser parser(m_config.locations, m_msg.m_method);
	if (!parser.parse_uri(m_msg.m_uri, m_uri))
	{
		m_answer.m_status_code = 404;
		return true;
	}
	m_location = parser.m_location;
	if (m_msg.m_error_num)																							// в стандарте указано, что если неспособность обработать запрос - временная, нужно отправить retry_after
	{
		m_answer.m_status_code = m_msg.m_error_num;
		return true;
	}
	int status;
	if ((status = check_validity()) != 0)
	{
		m_answer.m_status_code = status;
		return true;
	}
	if (m_msg.m_method == "GET")
		return (generate_GET());
	else if (m_msg.m_method == "POST")
		generate_POST();
	else if (m_msg.m_method == "DELETE")
		generate_DELETE();
	else if (m_msg.m_method == "HEAD")
		generate_HEAD();
	else if (m_msg.m_method == "PUT")
		generate_PUT();
//	else
//		wrong_method();
	return true;
}

int ft::ResponseHandler::check_validity()
{
	std::string& method = m_client.m_msg.m_method;
	if (method != "GET" && method != "POST" && method != "DELETE" && method != "HEAD" && method != "PUT")
	{
		m_client.m_answer.m_headers.push_back((http::Header){"Allow", "GET,POST,DELETE,HEAD,PUT"});
		return (501);
	}
	if (!m_location->allow.empty())
	{
		std::list<std::string>::const_iterator it = std::find(m_location->allow.begin(), m_location->allow.end(), method);
		if (it == m_location->allow.end())
		{
			std::string allowed;
			for (it = m_location->allow.begin(); it != m_location->allow.end(); ++it)
				allowed += *it + ",";
			allowed.erase(allowed.length() - 1);
			m_client.m_answer.m_headers.push_back((http::Header){"Allow", allowed});
			return (405);
		}
	}


	return 0;
}

bool ft::ResponseHandler::generate_GET()
{
	Answer& m_answer = m_client.m_answer;

	if (!m_location->cgi.second.empty() && m_location->cgi.first == m_uri.file_ext)
	{
		http::CgiHandler handler(m_config, m_client, m_uri);
		m_client.m_cgi_process = handler.spawn_cgi_process(*m_location);
		if (m_client.m_cgi_process.state() == http::CgiProcess::EError)
		{
			m_answer.m_status_code = 500;
			return true;
		}
		return false;
	}
	m_answer.m_status_code = 200;
	if (m_uri.extra_path.empty())
		generate_body();
	else
		m_answer.m_status_code = 404;
	if (m_answer.m_status_code == 200)
	{
		m_answer.m_headers.push_back((http::Header){"Connection", "Upgraded"});
		m_answer.m_headers.push_back((http::Header){"Last-Modified", detect_last_modified()});
	}
//	if (!m_answer.m_body.empty())
//		detect_content_type();
	return true;
}

bool ft::ResponseHandler::generate_HEAD()
{
//	bool ret;
//	ret = generate_GET();
//	m_answer.m_body.clear();
//	m_client.m_answer.m_body_exist = false;
	return generate_GET();
}

bool ft::ResponseHandler::generate_POST()
{
//    m_client.m_answer.m_status_code = 405; // по факту запрос уже валидный
//	// если запрос валидный, просто отвечать, что not implemented (код 501)/
//
////    m_client.m_answer.m_status_code = 201;
	Message& m_msg = m_client.m_msg;
	std::vector<http::Header>::iterator it = std::find_if(m_msg.m_headers.begin(),
														  m_msg.m_headers.end(),
														  http::FindHeader("content-type"));
	if (it != m_msg.m_headers.end() && it->value == "application/x-www-form-urlencoded")
	{
		return false;
	}
	m_client.m_answer.m_status_code = 501;
	return true;
}


bool ft::ResponseHandler::generate_PUT()
{
	// если создали файл, 201. если вписали в существующий - 200
	if (!m_uri.extra_path.empty())
		m_client.m_answer.m_status_code = 501;
	else
	{
		std::string file_name = m_uri.root + m_uri.path + m_uri.file_name;
		std::ios_base::openmode mode = std::ios_base::out;
		if (check_is_file(file_name))
		{
			mode |= std::ios_base::app;
			m_client.m_answer.m_status_code = 200;
		} else
			m_client.m_answer.m_status_code = 201;
		std::ofstream ofs(file_name, mode);
		if (ofs.is_open())
			ofs << m_client.m_msg.m_body;
		else
			m_client.m_answer.m_status_code = 403;
	}
	return true;
}

bool ft::ResponseHandler::generate_DELETE()
{
	if (!m_uri.extra_path.empty())
		m_client.m_answer.m_status_code = 501;
	else
	{
		std::string file_name = m_uri.root + m_uri.path + m_uri.file_name;
		if (check_is_file(file_name))
		{
//			std::ifstream file(file_name); // возможно нужна проверка на открытость файла
			if (std::remove(file_name.c_str()))
				m_client.m_answer.m_status_code = 205;
			else
				m_client.m_answer.m_status_code = 204;
		} else
			m_client.m_answer.m_status_code = 404;
	}
	return true;
}

bool ft::ResponseHandler::wrong_method()
{
    m_client.m_answer.m_status_code = 501;
	return true;
}

void ft::ResponseHandler::generate_body()
{
    Answer& m_answer = m_client.m_answer;
	std::ostringstream oss;
	std::ifstream file;
	DIR *dir;
	struct dirent *ent;

	const std::string full_path = m_uri.root + m_uri.path + m_uri.file_name;
	if (m_location->autoindex && (dir = opendir(full_path.c_str())) != NULL)
	{
		oss << BEFORE_BODY;
		while ((ent = readdir(dir)) != NULL)
			oss << "<a href='/" << m_uri.path + ent->d_name << "'>" << ent->d_name << "</a><br>";
		closedir (dir);
		oss << AFTER_BODY;
		m_answer.m_body = oss.str();
		m_uri.file_ext = "html";
	}
	else if (!from_file_to_body(full_path))
		m_answer.m_status_code = 404;

}


void ft::ResponseHandler::generate_status_body()
{
	Answer& m_answer = m_client.m_answer;
	Message& m_msg = m_client.m_msg;

	std::map<size_t, std::string>::iterator it = m_config.default_error_pages.find(m_answer.m_status_code);
	if (it != m_config.default_error_pages.end())
	{
		const std::string::size_type dot = it->second.find_last_of('.');
		if (dot != std::string::npos)
			m_uri.file_ext = it->second.substr(dot + 1);
		from_file_to_body(it->second);
	}
	else if (m_answer.m_status_code != 200 /* && m_msg.method == "GET" */)
	{
		m_uri.file_ext = "html";
		m_answer.m_body += http::default_status_body(m_answer.m_status_code);
	}
//	if (!m_answer.m_body.empty()) {
////		m_answer.m_headers.push_back((http::Header) {"Content-Length", Help::to_string(m_answer.m_body.size())});
//		m_answer.m_body_exist = true;
//	}
//	if (m_msg.m_method == "HEAD")
//		m_answer.m_body_exist = false;
	detect_content_type();
}

bool ft::ResponseHandler::from_file_to_body(const std::string &path)
{
    Answer& m_answer = m_client.m_answer;
	std::ostringstream			oss;
	std::ifstream				file;

	if (!check_is_file(path))
		return false;
	file.open(path);
	if (file.is_open())                                                                                                    //уточнить у Леши, нужно ли повторно чекать, что m_uri.file_name - это именно файл
	{
		oss << file.rdbuf();
		m_answer.m_body = oss.str();
		file.close();
		return true;
	}
	return false;
}

bool ft::ResponseHandler::check_is_file(const std::string &path)
{
	struct stat	buff;

	stat(path.c_str(), &buff);
	if ((buff.st_mode & S_IFMT) == S_IFREG)
			return true;
	return false;
}

std::string ft::ResponseHandler::detect_last_modified() {
	struct stat buff;
	std::string	file;
	file = m_uri.root + m_uri.path + m_uri.file_name;
	stat(file.c_str(), &buff);
	return (Help::get_date(buff.st_ctimespec));
}

void ft::ResponseHandler::detect_content_type()
{
	Answer& m_answer = m_client.m_answer;

	std::string content_type;
	content_type = ft::util::extension_to_mime_type(m_uri.file_ext);
	if (!content_type.empty())
		m_answer.m_headers.push_back((http::Header){"Content-Type", content_type});

}

