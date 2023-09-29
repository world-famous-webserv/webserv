#pragma once

#include <iostream>
#include <string>

class ASocket
{
	public:
		virtual ~ASocket()
		ASocket(const int port);
		ASocket(const int port, const char * address);
		ASocket(const ASocket & other);
		ASocket& operator = (const ASocket & other);

		virtual int handle();

	private:
		ASocket();

		int 		mPort;
		std::string	mAddress;
		int 		mSocketDesciptor;
}

class ServerSocket : public ASocket
{
	public:
		virtual ~ServerSocket();
		ServerSocket(const int port);
		ServerSocket(const int port, const char * address);
		ServerSocket(const ServerSocket & other);
		ServerSocket& operator = (const ServerSocket * other);

		virtual int	handle();

	private:
		ServerSocket();
}
