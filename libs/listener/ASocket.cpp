#include "ASocket.hpp"

ASocket::~ASocket()
{
	if (mSocket > 0)
		close(mSocket);
}

ASocket::ASocket()
	:
	mPort(-1),
	mAddress("")
{
}

ASocket::ASocket(const int port)
	:
	mPort(port),
	mAddress("")
{
}

ASocket::ASocket(const int port, const char * address)
	:
	mPort(port),
	mAddress(address)
{
}

ASocket::ASocket(const ASocket & other)
{
	*this = other;
}

ASocket & ASocket::operator = (const ASocket & other)
{
	if (this != &other)
	{
		this->mPort = other.mPort;
		this->mAddress = other.mAddress;
	}
	return (*this);
}
