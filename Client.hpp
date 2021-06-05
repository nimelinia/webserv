//
// Created by Streetmentioner Copycat on 5/8/21.
//

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include "Server.hpp"
#include "Message.hpp"

class	Message;

namespace ft {
	class Client {
	public:
		Client(int socketCl, Server &server);

		void fill_data(Message &msg);

		int m_socket_cl;
		int m_socket_serv;
		Server &m_server;
		size_t m_id;
//	bool	sending;
//	bool	receiving;

	};
}

#endif //CLIENT_HPP
