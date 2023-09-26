#pragma once

#ifndef LISTENER_HPP
#define LISTENER_HPP

#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "TcpStream.hpp"

#define MAX_CLIENTS 5
#define BUFFER_SIZE 1024

using namespace std;

class Listener {
	private:
		int		mServerSocket;
		string	mAddress;
		int		mPort;
		bool	mListening;

		Listener() {};

	public:
		Listener(int port, const char* address);
		Listener(int port);
		~Listener();

		int			start();
		TCPStream*  accept();
};

#endif