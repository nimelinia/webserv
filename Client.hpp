//
// Created by Streetmentioner Copycat on 5/8/21.
//

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>

class Client
{
public:
	int		*socket_cl;
	size_t	id;
	bool	sending;
	bool	receiving;

};


#endif //CLIENT_HPP
