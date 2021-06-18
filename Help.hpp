//
// Created by Natalia Troyanovskaya on 07.06.2021.
//
#pragma once
//#ifndef HELP_HPP
//#define HELP_HPP

#include <iostream>
#include <sstream>
#include <ctime>

namespace ft {
	class Help {
	public:
		static std::string get_date();
		static std::string get_date(timespec time);
		static std::string to_string(size_t num);
	};
}


//#endif //HELP_HPP
