#include "ASocket.hpp"

ASocket::~ASocket()
{
	if (mSocket > 0)
		close(mSocket);
}

ASocket::ASocket(const int port, const char * address)
	:
	mPort(port),
	mAddress(address)
{
}
