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
