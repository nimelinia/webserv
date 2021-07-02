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
			e_request_ready,
			e_response_ready,
			e_sending
		};
	public:
		Client(int socketCl, Server* server);
        virtual ~Client();

        void init_buffer();

		bool				read_message();
		bool				send_message();
        bool				send_cgi_message();
		void				close();
		bool				ready_write() const;
		bool				ready_read() const;
		int 				max_fd() const;

		bool cgi_spawned();
		void check_cgi();


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

