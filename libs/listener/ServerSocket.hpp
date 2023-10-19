#pragma once

#include "ASocket.hpp"
#include "ClientSocket.hpp"

class ServerSocket : public ASocket
{
	public:
		virtual ~ServerSocket();
		ServerSocket();
		ServerSocket(const int port);
		ServerSocket(const int port, const char * address);

		void	setSocket();
		void	accept();

	private:
		ServerSocket(const ServerSocket & other);
		ServerSocket & operator = (const ServerSocket * other);

};