//
// Created by Streetmentioner Copycat on 6/23/21.
//
#pragma once

#include "UriParser.hpp"

#define BEFORE_BODY "<html lang=\"en\"><head><meta charset=\"utf-8\"> \
					<title>Содержимое</title> \
					<style>body{margin: 40px; margin-left: 60px;font-size: 26px;font-family: \"PT Serif\",Georgia,Times New Roman,Times,serif;font-style: normal;} \
					</style></head><body>"
#define AFTER_BODY "</body></html>"


namespace ft
{
    class Client;

	class ResponseHandler
	{
	public:
		ResponseHandler(Config &config, Client& client);

		Uri						m_uri;
		Config&					m_config;
		Client&                 m_client;
		const Locations*		m_location;

		bool	generate_answer();
		int		check_validity();
		void	generate_GET();
		void	generate_body();
		void	generate_status_body();
		void	generate_HEAD();
		void	generate_POST();
		void	generate_DELETE();
		void	wrong_method();
//		bool	check_is_file(const std::string &path);
		bool	from_file_to_body(const std::string &path);
		std::string detect_last_modified();
		std::string detect_content_type();
	};

}
