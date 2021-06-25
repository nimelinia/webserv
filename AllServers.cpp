//
// Created by Streetmentioner Copycat on 5/22/21.
//

#include "AllServers.hpp"

ft::AllServers::AllServers()
{
}

ft::AllServers::~AllServers()
{
	// закрыть все серваки
}


void ft::AllServers::create_server(Config &config)
{
	ft::Server _server(config, config.port, config.hostaddress);
	m_servers.push_back(_server);
}


bool ft::AllServers::start_all_servers()
{
	for (std::list<Server>::iterator it = m_servers.begin(); it != m_servers.end(); ++it)
	{
		if (!it->create_server())
			return (false);
	}
	Select::get().set_max_fd(find_max_fd());

	for (;;)
	{
		if (Select::get().update())
		{
			bool update_max_fd = false;
			for (std::list<Server>:: iterator it = m_servers.begin(); it != m_servers.end(); ++it)
			{
				if (it->do_work())
					update_max_fd = true;
			}
			if (update_max_fd)
				Select::get().set_max_fd(find_max_fd());
		}
		usleep(1000000);																									// так долго, чтобы тестить
	}
	return (true);
}

int ft::AllServers::find_max_fd()
{
	int max_fd = 2;
	for (std::list<Server>::iterator max = m_servers.begin(); max != m_servers.end(); ++max)
		max_fd = std::max(max_fd, max->get_max_fd());
	return (max_fd);
}


//
//
//ssize_t ft::AllServers::write_to_socket(int index)
//{
//	Message	msg = m_clients_data[index].m_msg;
////	m_clients_data[index].m_answer->generate_answer(msg);
//	Answer	*answer = m_clients_data[index].m_answer;
//	ssize_t	ret;
//	ret = send(m_clients_data[index].m_socket_cl, answer->m_final_response.c_str(),  answer->m_size_response, 0);
//	if (ret == 0 || ret == -1)
//	{
//		// по идее можно просто сделать метод по закрытию сокета и здесь его вызывать
//		::close(m_open_sockets[index]);																						// закрываю сокет // или не надо??? поддерживаем подключение
//		FD_CLR(m_open_sockets[index], &m_clients);
//		m_open_sockets.erase(m_open_sockets.begin() + index);														// удаляю фд сокета из вектора
//		m_clients_data.erase(m_clients_data.begin() + index);
//	} else
//	{
//		answer->m_final_response.erase(0, ret);
//		if (answer->m_final_response.empty())																				// если не доотправлено, то в следующий раз по флажку пойдет отправлять
//		{
//			m_clients_data[index].m_msg.m_ready_responce = false;
//			m_clients_data[index].m_msg.clean();
//			m_clients_data[index].m_answer->clean();
//		}
//	}
//	return 0;
//}




