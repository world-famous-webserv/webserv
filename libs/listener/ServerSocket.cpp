#include "ServerSocket.hpp"

ServerSocket::~ServerSocket()
{
}

ServerSocket::ServerSocket()
	:
	ASocket(),
{
}

ServerSocket::ServerSocket(const int port)
	:
	ASocket(port),
{
}

ServerSocket::ServerSocket(const int port, const char * address)
	:
	ASocket(port, address),
{
}

ServerSocket::ServerSocket(const ServerSocket & other)
{
	*this = other;
}

ServerSocket & ServerSocket::operator = (const ServerSocket & other)
{
	if (this != &other)
	{
		this->mPort = other.mPort;
		this->mAddress = other.mAddress;
	}
	return (*this);
}