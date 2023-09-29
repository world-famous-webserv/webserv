#pragma once

#include <string>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#include "TcpStream.hpp"

#define MAX_CLIENTS 5
#define BUFFER_SIZE 1024

using namespace std;

class Listener
{
	public:
		~Listener();
		Listener(const int port);
		Listener(const int port, const char* address);
		int			start();
		TCPStream*  accept();

	private:
		Listener() {};

		int		mPort;
		string	mAddress;
		bool	mListening;
		int		mServerSocket;

};
