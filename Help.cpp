//
// Created by Natalia Troyanovskaya on 07.06.2021.
//

#include "Help.hpp"

std::string ft::Help::get_date() // нужно перевести в формат Sun, 06 Nov 1994 08:49:37 GMT ; IMF-fixdate
{
	time_t now = time(0);
	tm *ltm = localtime(&now);
	std::string			date_buf;
	date_buf = asctime(gmtime(&now));
	std::stringstream 	date;
	std::string			zero;
	int					i = 0;
	if (ltm->tm_mday < 10)
	{
		zero = "0";
		i = 1;
	}
	date << date_buf.substr(0, 3) + ", "
		 << zero + date_buf.substr(8 + i, 2 - i) + " "
		 << date_buf.substr(4, 3) + " "
		 << date_buf.substr(20, 4) + " "
		 << date_buf.substr(11, 8) + " GMT";
	return (date.str());
}

std::string ft::Help::to_string(size_t num)
{
	std::stringstream buf;
	buf << num;
	return (buf.str());
}

std::string ft::Help::get_date(struct timespec time)
{

//	time.tv_sec = time.tv_sec + time.tv_nsec / 1000000000;
	tm *ltm = localtime(&time.tv_sec);
	std::string			date_buf;
	date_buf = asctime(gmtime(&time.tv_sec));
	std::stringstream 	date;
	std::string			zero;
	int					i = 0;
	if (ltm->tm_mday < 10)
	{
		zero = "0";
		i = 1;
	}
	date << date_buf.substr(0, 3) + ", "
		 << zero + date_buf.substr(8 + i, 2 - i) + " "
		 << date_buf.substr(4, 3) + " "
		 << date_buf.substr(20, 4) + " "
		 << date_buf.substr(11, 8) + " GMT";
//	char date_1[256];
//	strftime(date_1, 256, "%c", ltm);
	return (date.str());
}
