#pragma once

#include <iostream>
#include <string>

class ASocket
{
	public:
		virtual ~ASocket()
		ASocket(const int port);
		ASocket(const int port, const char * address);

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
		virtual int	handle();

	private:
}
