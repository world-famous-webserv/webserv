#pragma once

#include "ASocket.hpp"

class ServerSocket : public ASocket
{
	public:
		virtual ~ServerSocket();
		ServerSocket();
		ServerSocket(const int port);
		ServerSocket(const int port, const char * address);
		ServerSocket(const ServerSocket & other);

		ServerSocket & operator = (const ServerSocket * other);

		virtual int	handle();
		void		setSocket();

};