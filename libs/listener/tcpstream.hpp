#pragma once

#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>

class TCPStream
{
	friend class Listener;
	public:
		~TCPStream();
		ssize_t 	send(char* buffer, size_t len);
		ssize_t 	receive(char* buffer, size_t len);

	private:
		TCPStream();
		TCPStream(int sd, struct sockaddr_in* address);
		TCPStream(const TCPStream& stream);

		std::string	mPeerIP;
		int			mPeerPort;
		int			mClientSocket;
};
