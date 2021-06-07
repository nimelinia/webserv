//
// Created by Streetmentioner Copycat on 5/8/21.
//

#include "Answer.hpp"

ft::Answer::Answer() :
	m_protocol_v("HTTP/1.1"),
	m_location(""),
	m_server("WebServer of dream-team/1.0"),
	m_content_type(""),
	m_content_length(0),
	m_date(Help::get_date()), // по идее это лучше делать позже, в момент формирования ответа - сам объект создается в момент создания сокета
	m_last_modified(""),
	m_transfer_encoding(""),
	m_body("")
{

}

void ft::Answer::generate_answer(ft::Message &message)
{

}

void ft::Answer::clean()
{

}
