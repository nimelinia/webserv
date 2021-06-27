//
// Created by Streetmentioner Copycat on 5/8/21.
//

#pragma once
//#ifndef CLIENT_HPP
//#define CLIENT_HPP

#include <iostream>
#include "Message.hpp"
#include "Answer.hpp"
#include "http/CgiHandler.hpp"

class	Answer;

namespace ft {
//	class Answer;
	class Server;
	class Client {

	private:
		enum Estate {
			e_request_parse,
			e_read_body,
			e_request_ready,
//			e_error,
			e_response_ready,
			e_sending
		};
	public:
		Client(int socketCl, Server* server);

		bool				read_message();
		bool				send_message();
//		void				read_body(ssize_t ret);
//		void				find_content_length();
		void				close();
		bool				ready_write() const;
		int 				max_fd() const;

		bool cgi_spawned();
		bool cgi_ready_read() const;
		bool cgi_ready_write() const;
		bool cgi_read();
		bool cgi_write();


		Estate				m_state;
		http::RequestParser	m_parser;
		http::CgiProcess	m_cgi_process;
		int					m_socket_cl;
		char*				m_buff;
		Message				m_msg;
		Server*				m_server;
		Answer				m_answer;
		size_t				m_parsed;
		size_t				m_content_length;
		bool				m_delete_me;
//	bool	sending;
//	bool	receiving;

	};
}

//#endif //CLIENT_HPP

