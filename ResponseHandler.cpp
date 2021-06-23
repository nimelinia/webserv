//
// Created by Streetmentioner Copycat on 6/23/21.
//

#include "ResponseHandler.hpp"

ft::ResponseHandler::ResponseHandler(Config &config, Message& msg, Answer &answer) :
	m_config(config),
	m_msg(msg),
	m_answer(answer)
{

}

void ft::ResponseHandler::generate_answer()
{
	UriParser parser(m_config.locations);
	if (!parser.parse_uri(m_msg.m_uri, m_uri))
	{
		m_answer.m_status_code = 404;
		return;
	}

	if (m_msg.m_error_num)																							// в стандарте указано, что если неспособность обработать запрос - временная, нужно отправить retry_after
	{
		m_answer.m_status_code = m_msg.m_error_num;
		return;
	}
	int status;
	if ((status = check_validity(m_msg)) != 0)
	{
		m_answer.m_status_code = status;
		return;
	}
	m_answer.m_headers.push_back(http::Header("name", "value"));
	if (m_msg.m_method == "GET")
		generate_GET();
	else if (m_msg.m_method == "POST")
		generate_POST();
//		generate_GET();
	else if (m_msg.m_method == "DELETE")
		generate_DELETE();
	else
		wrong_method();
}
