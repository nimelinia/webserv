//
// Created by Streetmentioner Copycat on 5/8/21.
//

#include "Client.hpp"
#include "Server.hpp"
#include "http/RequestParser.h"
#include "ResponseHandler.hpp"
#include "log/Log.h"

ft::Client::Client(int socketCl, Server *server) :
	m_state(e_request_parse),
	m_socket_cl(socketCl),
	m_msg(NULL),
	m_server(server),
	m_answer(NULL),		// тут нужно поменять, т.к. этот лимит лежит внутри location и мы сначала должны распарсить uri, а потом уже определять размер
	m_parsed(),
	m_buff(NULL)
{
}

ft::Client::~Client()
{
    delete[] m_buff;
	delete m_msg;
	delete m_answer;
}

bool ft::Client::read_message()
{
	ssize_t	ret;

	ret = recv(m_socket_cl, m_buff, BUFFER_SIZE, 0);
	if (ret == 0 || ret == -1)
		return (true);
	if (m_state == e_request_parse)
	{
        http::RequestParser::EResult res = m_parser.parse(m_server->m_configs, *m_msg, m_buff, ret);
        if (res == http::RequestParser::EParse)
			return false;

        struct sockaddr_in peer;
        socklen_t peer_len  = sizeof(peer);
        ::getpeername(m_socket_cl, (struct sockaddr*)&peer, &peer_len);
        LOGD << "READ Client " << ::inet_ntoa(peer.sin_addr) << ":" << peer.sin_port;

		if (res == http::RequestParser::EOk)
		{
			LOGD << "URI: " << m_msg->m_uri_str;
			LOGD << "Method: " << m_msg->m_method;
			LOGD << "Headers:";
			for (std::vector<http::Header>::iterator hit = m_msg->m_headers.begin(); hit != m_msg->m_headers.end(); ++hit)
				LOGD << "\t" << hit->name << ": " << hit->value;
			LOGD;
			m_answer->m_server = m_msg->m_uri.config->server_name;
			m_state = e_request_ready;
		}
		else if (res == http::RequestParser::EError)
		{
			if (!m_msg->m_error_num)
                LOGE << "Error parsing header";
			m_answer->m_status_code = m_msg->m_error_num;
			m_state = e_request_ready;
		}

        std::vector<http::Header>::iterator it = std::find_if(m_msg->m_headers.begin(),
                m_msg->m_headers.end(),
                http::FindHeader("connection"));
		m_delete_me = (it != m_msg->m_headers.end() && it->value == "close");
	}
	if (m_state == e_request_ready)
	{
		ResponseHandler handler(*m_msg->m_uri.config, *this);
		bool result = true;
		if (!m_answer->m_status_code)
			result = handler.generate_answer();
		if (result)
		{
			handler.generate_status_body();
			delete m_msg;
			m_msg = NULL;
			m_state = e_response_ready;
		}

	}
	return (false);
}

bool ft::Client::send_message()
{
    if (m_state == e_response_ready) {
        if (m_cgi_process.state() == http::CgiProcess::ESpawn)
        {
            http::CgiHandler handler(m_server->m_configs.front(), *this, m_msg->m_uri);
            if (!handler.parse_cgi_body())
                m_answer->m_status_code = 500;
            if (m_answer->m_status_code == 0)
                m_answer->m_status_code = 200;
        }
        else
		    m_answer->m_headers.push_back((http::Header) {"Content-Length",
													 util::str::ToString(m_answer->m_body.size())});

        struct sockaddr_in peer;
        socklen_t peer_len  = sizeof(peer);
        ::getpeername(m_socket_cl, (struct sockaddr*)&peer, &peer_len);
        LOGD << "Client " << ::inet_ntoa(peer.sin_addr) << ":" << peer.sin_port;

        m_answer->create_final_response();
		m_state = e_sending;
	}
	ssize_t	ret;
    if (!m_answer->m_final_response.empty())
	    ret = ::send(m_socket_cl, m_answer->m_final_response.c_str(),  m_answer->m_final_response.size(), 0);
    else
	    ret = _send_cgi_body();
	if (ret == -1) {
		return (true);
	}
	else if (ret == 0)
		return (false);
	// если не доотправлено, то в следующий раз по флажку пойдет отправлять
	if (!m_answer->m_final_response.empty())
	    m_answer->m_final_response.erase(0, ret);
	if (m_answer->m_final_response.empty() && m_cgi_process.state() != http::CgiProcess::ESpawn)																				// если не доотправлено, то в следующий раз по флажку пойдет отправлять
	{
	    const bool close_on_error = (m_answer->m_status_code == 400 || m_answer->m_status_code == 413);
//		delete m_msg;
	    m_msg = new Message();
//	    m_msg.m_ready_responce = false;
		m_parser.reset();
//		m_msg.clean();
//		m_answer.clean();
		delete m_answer;
		m_answer = new Answer();
		m_cgi_process.clear();
		m_state = e_request_parse;
		return close_on_error || m_delete_me;
	}
	return (false);
}


void ft::Client::close()
{
	::close(m_socket_cl);																						// закрываю сокет
	Select::get().clear_fd(m_socket_cl);
}

bool ft::Client::ready_read() const
{
	return (m_state == e_request_parse);
}

bool ft::Client::ready_write() const
{
	return (m_state == e_response_ready || m_state == e_sending);
}

int ft::Client::max_fd() const
{
    return std::max(m_socket_cl, m_cgi_process.max_fd());
}

void ft::Client::init_buffer()
{
    m_buff = new char[BUFFER_SIZE];
    m_answer = new Answer;
    m_msg = new Message;
}
void ft::Client::check_cgi()
{
    if (m_cgi_process.is_done())
    {
        m_state = e_response_ready;
        delete m_msg;
        m_msg = NULL;
        LOGI_(CGI) << "Cgi process finished";
    }
}

ssize_t ft::Client::_send_cgi_body()
{
    if (m_cgi_process.bytes_written == 0)
    {
        m_cgi_process.bytes_read = std::fread(m_buff, 1, BUFFER_SIZE, m_cgi_process.read_file);
        if (std::ferror(m_cgi_process.read_file))
            return -1;
    }
    ssize_t ret = 0;
    if (m_cgi_process.bytes_read > 0)
        ret = ::send(m_socket_cl, m_buff + m_cgi_process.bytes_written,  m_cgi_process.bytes_read - m_cgi_process.bytes_written, 0);
    if (ret > 0)
        m_cgi_process.bytes_written += ret;
    if (m_cgi_process.bytes_written == m_cgi_process.bytes_read)
        m_cgi_process.bytes_written = 0;
    if (ret == 0 && std::feof(m_cgi_process.read_file))
    {
        m_cgi_process.clear();
        return 1;
    }
    return ret;
}
