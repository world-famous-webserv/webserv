#include "ClientSocket.hpp"

ClientSocket::~ClientSocket()
{
}

ClientSocket::ClientSocket()
	:
	ASocket()
{
}

ClientSocket::ClientSocket(const int sd,  struct sockaddr_in * address)
	:
	ASocket()
{
	mSocket = sd;
	char ip[50];
	// 사용불가함수
	inet_ntop(PF_INET, reinterpret_cast<const struct in_addr*>(&address->sin_addr.s_addr), ip, sizeof(ip) - 1);
	mIp = ip;
	mPort = ntohs(address->sin_port);
	std::cout << "Client mIP: " << mIp << "\n";
	std::cout << "Client mPort: " << mPort << "\n";
}

ssize_t	ClientSocket::send(char* buffer, size_t len)
{
	return write(mSocket, buffer, len);
}

ssize_t	ClientSocket::receive(char* buffer, size_t len)
{
	return read(mSocket, buffer, len);
}
