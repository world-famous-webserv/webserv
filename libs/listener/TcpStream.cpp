#include "TcpStream.hpp"
#include <iostream>

TCPStream::~TCPStream()
{
	close(mClientSocket);
}

TCPStream::TCPStream(int sd, struct sockaddr_in* address)
	:mClientSocket(sd)
{
	char ip[50];
	// 사용불가함수
	inet_ntop(PF_INET, reinterpret_cast<const struct in_addr*>(&address->sin_addr.s_addr), ip, sizeof(ip) - 1);
	mPeerIP = ip;
	mPeerPort = ntohs(address->sin_port);
	std::cout << "mPeerIP: " << mPeerIP << "\n";
	std::cout << "mPeerPort: " << mPeerPort << "\n";
}

ssize_t	TCPStream::send(char* buffer, size_t len)
{
	return write(mClientSocket, buffer, len);
}

ssize_t	TCPStream::receive(char* buffer, size_t len)
{
	return read(mClientSocket, buffer, len);
}
