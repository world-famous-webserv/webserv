#pragma once
 
#include <iostream>
#include <string>
#include <unistd.h>

class ASocket
{
	public:
		virtual ~ASocket();
		ASocket();
		ASocket(const int port);
		ASocket(const ASocket & other);
		ASocket(const int port, const char * address);

		ASocket& operator = (const ASocket & other);

		virtual int handle();

	private:
		int 		mPort;
		std::string	mAddress;
		int 		mSocket;
};
