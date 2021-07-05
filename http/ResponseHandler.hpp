//
// Created by Streetmentioner Copycat on 6/23/21.
//
#pragma once

#include "http/parsers/UriParser.hpp"

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
		ResponseHandler(Config& config, Client& client);

		bool					generate_answer();
		void					generate_status_body();

	private:
		Config&					m_config;
		Client&                 m_client;

		size_t 					_check_validity();
		void					_generate_body();
		bool 					_generate_GET();
		bool 					_generate_HEAD();
		bool					_generate_POST();
		bool					_generate_PUT();
		bool					_generate_DELETE();
		bool					_check_is_file(const std::string &path);
		bool					_from_file_to_body(const std::string &path);
		bool 					_detect_content_type();
		std::string				_detect_last_modified();
	};

}
