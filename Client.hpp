//
// Created by Streetmentioner Copycat on 5/8/21.
//

#pragma once
//#ifndef CLIENT_HPP
//#define CLIENT_HPP

#include <iostream>
#include "Message.hpp"
#include "Answer.hpp"
class	Answer;

namespace ft {
//	class Answer;
	class Server;
	class Client {

	private:
		enum Estate {
			e_parse_header,
			e_read_body,
			e_request_ready,
			e_error,
			e_response_ready
		};
	public:
		Client(int socketCl, Server* server);

		bool				read_message();
		bool				send_message();
		void				read_body(ssize_t ret);
		void				find_content_length();
		void				close();
		bool				ready_write() const;


		Estate				m_state;
		http::RequestParser	m_parser;
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

