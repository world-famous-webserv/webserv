#pragma once

#include "ASocket.hpp"

class ClientSocket : public ASocket
{
	public:
		virtual ~ClientSocket();
		ClientSocket(const int sd, struct sockaddr_in * address);

		ssize_t		send(char* buffer, size_t len);
		ssize_t		receive(char* buffer, size_t len);

	private:
		ClientSocket();
		ClientSocket(const ClientSocket & other);
		ClientSocket & operator=(const ClientSocket * other);
};
