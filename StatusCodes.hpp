#ifndef STATUSCODES_HPP
#define STATUSCODES_HPP

#include <string>

namespace ft { namespace http
{
	const char* status_to_string(size_t status);
    const char* default_status_body(size_t status);
} }

#endif //STATUSCODES_HPP
