//
// Created by Streetmentioner Copycat on 6/23/21.
//

#include "ResponseHandler.hpp"
#include "Client.hpp"
#include "MimeTypes.hpp"
#include "StatusCodes.hpp"

ft::ResponseHandler::ResponseHandler(Config& config, Client& client) :
	m_config(config),
	m_client(client)
{
}

bool ft::ResponseHandler::generate_answer()
{
    Message& m_msg = m_client.m_msg;
    Answer& m_answer = m_client.m_answer;

	if (m_msg.m_error_num)																									// в стандарте указано, что если неспособность обработать запрос - временная, нужно отправить retry_after
	{
		m_answer.m_status_code = m_msg.m_error_num;
		return true;
	}
	size_t status;
	if ((status = _check_validity()) != 0)
	{
		m_answer.m_status_code = status;
		return true;
	}
	if (m_msg.m_method == "GET")
		return (_generate_GET());
	else if (m_msg.m_method == "POST")
		return (_generate_POST());
	else if (m_msg.m_method == "DELETE")
		return (_generate_DELETE());
	else if (m_msg.m_method == "HEAD")
		return (_generate_HEAD());
	else if (m_msg.m_method == "PUT")
		return (_generate_PUT());
	return true;
}

size_t ft::ResponseHandler::_check_validity()
{

	std::string& method = m_client.m_msg.m_method;
	if (method != "GET" && method != "POST" && method != "DELETE" && method != "HEAD" && method != "PUT")
	{
		m_client.m_answer.m_headers.push_back((http::Header){"Allow", "GET,POST,DELETE,HEAD,PUT"});
		return (501);
	}
	if (!m_client.m_msg.m_uri.locations->allow.empty())
	{
		std::list<std::string>::const_iterator it = std::find(m_client.m_msg.m_uri.locations->allow.begin(), \
						m_client.m_msg.m_uri.locations->allow.end(), method);
		if (it == m_client.m_msg.m_uri.locations->allow.end())
		{
			std::string allowed;
			for (it = m_client.m_msg.m_uri.locations->allow.begin(); it != m_client.m_msg.m_uri.locations->allow.end(); ++it)
				allowed += *it + ",";
			allowed.erase(allowed.length() - 1);
			m_client.m_answer.m_headers.push_back((http::Header){"Allow", allowed});
			return (405);
		}
	}
	if (m_client.m_msg.m_uri.config->redirection.first != 0)
	{
		m_client.m_answer.m_headers.push_back((http::Header){"Location", m_client.m_msg.m_uri.config->redirection.second});
		return (m_client.m_msg.m_uri.config->redirection.first);
	}
	return 0;
}

bool ft::ResponseHandler::_generate_GET()
{
	Answer& m_answer = m_client.m_answer;

	if (!m_client.m_msg.m_uri.locations->cgi.second.empty() && m_client.m_msg.m_uri.locations->cgi.first == \
			m_client.m_msg.m_uri.file_ext)
	{
		http::CgiHandler handler(m_config, m_client, m_client.m_msg.m_uri);
		m_client.m_cgi_process = handler.spawn_cgi_process(*m_client.m_msg.m_uri.locations);
		if (m_client.m_cgi_process.state() == http::CgiProcess::EError)
		{
			m_answer.m_status_code = 500;
			return true;
		}
		return false;
	}
	m_answer.m_status_code = 200;
	if (m_client.m_msg.m_uri.extra_path.empty())
		_generate_body();
	else
		m_answer.m_status_code = 404;
	if (m_answer.m_status_code == 200)
	{
		m_answer.m_headers.push_back((http::Header){"Connection", "Upgraded"});
		m_answer.m_headers.push_back((http::Header){"Last-Modified", _detect_last_modified()});
	}
	return true;
}

bool ft::ResponseHandler::_generate_HEAD()
{
	return _generate_GET();
}

bool ft::ResponseHandler::_generate_POST()
{
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

bool ft::ResponseHandler::_generate_PUT()
{
	if (!m_client.m_msg.m_uri.extra_path.empty())
		m_client.m_answer.m_status_code = 501;
	else
	{
		std::string file_name = m_client.m_msg.m_uri.root + m_client.m_msg.m_uri.path + m_client.m_msg.m_uri.file_name;
		std::ios_base::openmode mode = std::ios_base::out;
		if (_check_is_file(file_name))
		{
			mode |= std::ios_base::app;
			m_client.m_answer.m_status_code = 200;
		} else
			m_client.m_answer.m_status_code = 201;
		std::ofstream ofs(file_name, mode);
		if (ofs.is_open())
		{
			ofs << m_client.m_msg.m_body;
			m_client.m_answer.m_headers.push_back((http::Header){"Content-Location", \
						m_client.m_msg.m_uri.locations->path_to_location.front() + m_client.m_msg.m_uri.path + m_client.m_msg.m_uri.file_name});
		}
		else
			m_client.m_answer.m_status_code = 403;
	}
	return true;
}

bool ft::ResponseHandler::_generate_DELETE()
{
	if (!m_client.m_msg.m_uri.extra_path.empty())
		m_client.m_answer.m_status_code = 501;
	else
	{
		std::string file_name = m_client.m_msg.m_uri.root + m_client.m_msg.m_uri.path + m_client.m_msg.m_uri.file_name;
		if (_check_is_file(file_name))
		{
			if (std::remove(file_name.c_str()))
				m_client.m_answer.m_status_code = 205;
			else
				m_client.m_answer.m_status_code = 204;
		} else
			m_client.m_answer.m_status_code = 404;
	}
	return true;
}

void ft::ResponseHandler::_generate_body()
{
    Answer& m_answer = m_client.m_answer;
	std::ostringstream oss;
	std::ifstream file;
	DIR *dir;
	struct dirent *ent;

	const std::string full_path = m_client.m_msg.m_uri.root + m_client.m_msg.m_uri.path + m_client.m_msg.m_uri.file_name;
	if (m_client.m_msg.m_uri.locations->autoindex && (dir = opendir(full_path.c_str())) != NULL)
	{
		oss << BEFORE_BODY;
		while ((ent = readdir(dir)) != NULL)
			oss << "<a href='/" << m_client.m_msg.m_uri.locations->path_to_location.front().substr(1) + m_client.m_msg.m_uri.path\
						+ ent->d_name << "'>" << ent->d_name << "</a><br>";
		closedir (dir);
		oss << AFTER_BODY;
		m_answer.m_body = oss.str();
		m_client.m_msg.m_uri.file_ext = "html";
	}
	else if (!_from_file_to_body(full_path))
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
			m_client.m_msg.m_uri.file_ext = it->second.substr(dot + 1);
		_from_file_to_body(it->second);
	}
	else if (m_answer.m_status_code != 200 /* && m_msg.method == "GET" */)
	{
		m_client.m_msg.m_uri.file_ext = "html";
		m_answer.m_body += http::default_status_body(m_answer.m_status_code);
	}
	if (!_detect_content_type())
		generate_status_body();
}

bool ft::ResponseHandler::_from_file_to_body(const std::string &path)
{
    Answer& m_answer = m_client.m_answer;
	std::ostringstream			oss;
	std::ifstream				file;

	if (!_check_is_file(path))
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

bool ft::ResponseHandler::_check_is_file(const std::string &path)
{
	struct stat	buff;

	stat(path.c_str(), &buff);
	if ((buff.st_mode & S_IFMT) == S_IFREG)
			return true;
	return false;
}

std::string ft::ResponseHandler::_detect_last_modified() {
	struct stat buff;
	std::string	file;
	file = m_client.m_msg.m_uri.root + m_client.m_msg.m_uri.path + m_client.m_msg.m_uri.file_name;
	stat(file.c_str(), &buff);
	return (Help::get_date(buff.st_ctimespec));
}

bool ft::ResponseHandler::_detect_content_type()
{
	Answer& m_answer = m_client.m_answer;

	std::string content_type;
	content_type = ft::util::extension_to_mime_type(m_client.m_msg.m_uri.file_ext);
	if (!content_type.empty())
	{
		m_answer.m_headers.push_back((http::Header){"Content-Type", content_type});
		return (true);
	}
	m_answer.m_status_code = 415;
	return (false);
}

