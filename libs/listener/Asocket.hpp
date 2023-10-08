#pragma once
 
#include <iostream>
#include <cstring>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cerrno>

#define MAXCLIENT 5
#define BUFFER_SIZE 1024

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
