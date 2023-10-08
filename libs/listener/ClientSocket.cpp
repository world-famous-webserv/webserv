#include "ClientSocket.hpp"

ClientSocket::~ClientSocket()
{
}

ClientSocket::ClientSocket()
	:
	ASocket(),
{
}

ClientSocket::ClientSocket(const int port)
	:
	ASocket(port),
{
}

ClientSocket::ClientSocket(const int port, const char * address)
	:
	ASocket(port, address),
{
}

ClientSocket::ClientSocket(const ClientSocket & other)
{
	*this = other;
}

ClientSocket & ClientSocket::operator = (const ClientSocket & other)
{
	if (this != &other)
	{
		this->mPort = other.mPort;
		this->mAddress = other.mAddress;
	}
	return (*this);
}