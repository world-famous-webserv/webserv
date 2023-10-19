#pragma once
 
#include <cstring>
#include <string>
#include <cerrno>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define MAX_CLIENT 5
#define BUFFER_SIZE 1024
#define LINE_BUFFER_SIZE 512

class ASocket
{
	public:
		virtual ~ASocket();
		ASocket();
		ASocket(const int port);
		ASocket(const int port, const char * address);

	private:
		ASocket(const ASocket & other);
		ASocket& operator = (const ASocket & other);

	protected:
		int 		mPort;
		int 		mSocket;
		std::string	mAddress;
};

