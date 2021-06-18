//
// Created by Streetmentioner Copycat on 5/8/21.
//

#pragma once
//#ifndef CLIENT_HPP
//#define CLIENT_HPP

#include <iostream>
#include "Server.hpp"
#include "Message.hpp"
#include "Answer.hpp"

class	Answer;

namespace ft {
//	class Answer;
	class Client {
	public:
		Client(int socketCl, Server *server);

		int		m_socket_cl;
		int		m_socket_serv;
		Message	m_msg;
		Server	*m_server;
		Answer	*m_answer;
		size_t	m_id;
//	bool	sending;
//	bool	receiving;

	};
}

//#endif //CLIENT_HPP

