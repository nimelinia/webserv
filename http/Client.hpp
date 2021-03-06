//
// Created by Streetmentioner Copycat on 5/8/21.
//

#pragma once
//#ifndef CLIENT_HPP
//#define CLIENT_HPP

#include <iostream>
#include "http/Message.hpp"
#include "http/Answer.hpp"
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
		void				close();
		bool				ready_write() const;
		bool				ready_read() const;
		int 				max_fd() const;

		void check_cgi();


		Estate				m_state;
		http::RequestParser	m_parser;
		http::CgiProcess	m_cgi_process;
		int					m_socket_cl;
		char*				m_buff;
		Message*			m_msg;
		Server*				m_server;
		Answer*				m_answer;
		size_t				m_parsed;
		size_t				m_content_length;
		bool				m_delete_me;
		time_t 				m_last_action_time;

    private:
	    ssize_t _send_cgi_body();
	};
}

//#endif //CLIENT_HPP

