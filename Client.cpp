//
// Created by Streetmentioner Copycat on 5/8/21.
//

#include "Client.hpp"
//#include "Server.hpp"


ft::Client::Client(int socketCl, Server *server) :
	m_socket_cl(socketCl),
	m_server(server),
	m_answer(new Answer(&server->m_config))
{

}
