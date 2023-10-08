#pragma once

#include "ASocket.hpp"

class ClientSocket : public ASocket
{
	public:
		virtual ~ClientSocket();
		ClientSocket();
		ClientSocket(const int port);
		ClientSocket(const int port, const char * address);
		ClientSocket(const ClientSocket & other);

		ClientSocket & operator = (const ClientSocket * other);

		virtual int	handle();

};