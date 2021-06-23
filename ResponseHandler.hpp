//
// Created by Streetmentioner Copycat on 6/23/21.
//

#ifndef RESPONSEHANDLER_HPP
#define RESPONSEHANDLER_HPP

#include "UriParser.hpp"
#include "Answer.hpp"
#include "Message.hpp"

namespace ft{
	class ResponseHandler
	{
	public:
		ResponseHandler(Config &config, Message& msg, Answer &answer);

		Uri		m_uri;
//		std::list<Locations>&	m_locations;
		Config&		m_config;
		Message&	m_msg;
		Answer&		m_answer;

		void generate_answer();


	};

}



#endif //RESPONSEHANDLER_HPP
