#include "ServerSocket.hpp"

ServerSocket::~ServerSocket()
{
}

ServerSocket::ServerSocket()
	:
	ASocket()
{
}

ServerSocket::ServerSocket(const int port)
	:
	ASocket(port)
{
}

ServerSocket::ServerSocket(const int port, const char * address)
	:
	ASocket(port, address)
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

void ServerSocket::setSocket()
{
	struct sockaddr_in address = {};

	address.sin_family = AF_INET;
	address.sin_port = htons(mPort);
	address.sin_addr.s_addr = INADDR_ANY;

	mSocket = socket(AF_INET, SOCK_STREAM, 0);

	const int optval = 1;
	setsockopt(mSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

	if (0 != bind(mSocket, reinterpret_cast<const struct sockaddr*>(&address), sizeof(address)))
		throw std::runtime_error("Error: " + strerror(errno));
	if (0 != listen(mSocket, MAX_CLIENTS))
		throw std::runtime_error("Error: " + strerror(errno));
}