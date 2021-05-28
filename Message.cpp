//
// Created by Streetmentioner Copycat on 5/8/21.
//

#include "Message.hpp"

Message::Message()
{

}

void Message::copy_buff(char *buf)
{
	if (!buf)
		return;
	for (size_t i = 0; buf[i] != '\0'; ++i)
		m_buff[i] = buf[i];
}

void Message::parse()
{

}

void Message::clean()
{
	m_buff[0] = '\0';
}
