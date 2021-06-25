//
// Created by Streetmentioner Copycat on 6/23/21.
//

#include "ResponseHandler.hpp"
#include "Client.hpp"

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

	UriParser parser(m_config.locations);
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
    {
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
        generate_GET();
    }
	else if (m_msg.m_method == "POST")
		generate_POST();
	else if (m_msg.m_method == "DELETE")
		generate_DELETE();
	else if (m_msg.m_method == "HEAD")
		generate_HEAD();
	else
		wrong_method();
	return true;
}

int ft::ResponseHandler::check_validity()
{
	return 0;
}

void ft::ResponseHandler::generate_GET()
{
    Answer& m_answer = m_client.m_answer;
	m_answer.m_status_code = 200;
	generate_body();
	if (m_answer.m_status_code == 200)
	{
		m_answer.m_headers.push_back((http::Header){"Connection", "Upgraded"});
		m_answer.m_headers.push_back((http::Header){"Last-Modified", detect_last_modified()});
	}
	if (m_answer.m_body_exist)
	{
		m_answer.m_headers.push_back((http::Header){"Content-Length", Help::to_string(m_answer.m_body.size())});
		m_answer.m_headers.push_back((http::Header){"Content-Type", detect_content_type()});
	}
}

void ft::ResponseHandler::generate_HEAD()
{
	generate_GET();
//	m_answer.m_body.clear();
	m_client.m_answer.m_body_exist = false;
}

void ft::ResponseHandler::generate_POST()
{
    m_client.m_answer.m_status_code = 200;


    m_client.m_answer.m_status_code = 201;
}

void ft::ResponseHandler::generate_DELETE()
{
    m_client.m_answer.m_status_code = 200;

    m_client.m_answer.m_status_code = 202;

    m_client.m_answer.m_status_code = 204;
}

void ft::ResponseHandler::wrong_method()
{
    m_client.m_answer.m_status_code = 501;
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
		m_answer.m_body_exist = true;
	}
	else if (!from_file_to_body(full_path))
		m_answer.m_status_code = 403;
}


void ft::ResponseHandler::generate_status_body()
{
    Answer& m_answer = m_client.m_answer;
	const bool body_exist = m_answer.m_body_exist;
	/*
	 * if (custom status page)
	 * 		from_file_to_body(status_page);
	 * else
	 * 		m_answer.m_body += http::default_status_body(m_answer.m_status_code);
	 */
	m_answer.m_body_exist = body_exist;
}

bool ft::ResponseHandler::from_file_to_body(const std::string &path)
{
    Answer& m_answer = m_client.m_answer;
	std::ostringstream			oss;
	std::ifstream				file;

	file.open(path);
	if (file.is_open())                                                                                                    //уточнить у Леши, нужно ли повторно чекать, что m_uri.file_name - это именно файл
	{
		oss << file.rdbuf();
		m_answer.m_body = oss.str();
		m_answer.m_body_exist = true;
		file.close();
		return true;
	}
	return false;
}

//bool ft::ResponseHandler::check_is_file(const std::string &path)
//{
//	struct stat	buff;
//
//	stat(path.c_str(), &buff);
//	if ((buff.st_mode & S_IFMT) == S_IFREG)
//			return true;
//	return false;
//}

std::string ft::ResponseHandler::detect_last_modified()
{
	struct stat buff;
	std::string	file;
	file = m_uri.path + m_uri.file_name;
	stat(file.c_str(), &buff);
	return (Help::get_date(buff.st_ctimespec));
}

std::string ft::ResponseHandler::detect_content_type()
{
	return ("");
}
