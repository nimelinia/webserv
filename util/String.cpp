#include "String.hpp"

std::list<std::string> ft::util::str::Split(const std::string& str, char ch)
{
    std::list<std::string> split;
    const char* start = str.c_str();
    const char* cur = start;

    while (*cur != '\0')
    {
        if (*cur == ch)
        {
            split.push_back(std::string(start, cur - start));
            start = cur + 1;
        }
        cur++;
    }

    if (start != cur)
        split.push_back(std::string(start, cur - start));

    return split;
}

std::string ft::util::date::get_date()
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

std::string ft::util::date::get_date(struct timespec time)
{
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
	return (date.str());
}
