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
		throw std::runtime_error(strerror(errno));
	if (0 != listen(mSocket, MAX_CLIENT))
		throw std::runtime_error(strerror(errno));
}

void ServerSocket::accept()
{
	struct sockaddr_in	address;
	memset(&address, 0, sizeof(address));
	socklen_t 			sockLen = sizeof(address);
	const int 			sd = ::accept(mSocket, reinterpret_cast<struct sockaddr*>(&address), &sockLen);

	if (sd < 0)
		throw std::runtime_error(strerror(errno));

	ClientSocket*	clientSocket = new ClientSocket(sd, &address);
	ssize_t			len = 0;
	char			line[LINE_BUFFER_SIZE];

	if (clientSocket != NULL) {
		while ((0 < (len = clientSocket->receive(line, sizeof(line))))) {
			line[len] = '\0';

			std::cout << "ServerSocket.cpp = len - "<< strlen(line) << " received - \"" << line << "\"" << std::endl;

			clientSocket->send(line, len);
		}
		if (0 > len)
			throw std::runtime_error(strerror(errno));
		delete clientSocket;
	}
}
