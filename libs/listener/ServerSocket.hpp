#ifndef SERVERSOCKET_HPP
#define SERVERSOCKET_HPP

#include "ASocket.hpp"
#include "ClientSocket.hpp"

class ServerSocket : public ASocket
{
	public:
		virtual ~ServerSocket();
		ServerSocket(const int port);
		ServerSocket(const int port, const char * address);

		void	accept();
		void	setSocket();

	private:
		ServerSocket();
		ServerSocket(const ServerSocket & other);
		ServerSocket & operator = (const ServerSocket * other);

};

#endif
