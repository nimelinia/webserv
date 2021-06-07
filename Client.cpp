//
// Created by Streetmentioner Copycat on 5/8/21.
//

#include "Client.hpp"
//#include "Server.hpp"


ft::Client::Client(int socketCl, Server &server, Message &msg, Answer &answer) :
	m_socket_cl(socketCl),
	m_server(server),
	m_msg(msg),
	m_answer(answer)
{

}
