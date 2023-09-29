#include "Listener.hpp"

Listener::~Listener()
{
	if (mServerSocket > 0)
		close(mServerSocket);
}

Listener::Listener(const int port)
	:mServerSocket(0),
	mAddress(""),
	mPort(port),
	mListening(false)
{
}

Listener::Listener(const int port, const char* address)
	:mServerSocket(0),
	mAddress(address),
	mPort(port),
	mListening(false)
{
}

int Listener::start()
{
	if (mListening == true)
		return 0;

	mServerSocket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in address;
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(mPort);
	address.sin_addr.s_addr = INADDR_ANY;

	const int optval = 1;
	setsockopt(mServerSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

	const int bind_result = bind(mServerSocket, \
			reinterpret_cast<const struct sockaddr*>(&address), sizeof(address));
	if (bind_result != 0) {
		std::cout << "Error: " << strerror(errno);
		return bind_result;
	}
	const int listen_result = listen(mServerSocket, MAX_CLIENTS);
	if (listen_result != 0) {
		std::cout << "Error: " << strerror(errno);
		return listen_result;
	}
	mListening = true;
	return listen_result;
}

TCPStream* Listener::accept()
{
	if (mListening == false) 
		return NULL;

	struct sockaddr_in address;
	socklen_t len = sizeof(address);
	memset(&address, 0, sizeof(address));
	
	const int sd = ::accept(mServerSocket, \
			reinterpret_cast<struct sockaddr*>(&address), &len);
	std::cout << "Listener::accept(): sd: " << sd << "\n";

	if (sd < 0) {
		std::cout << "Error: " << strerror(errno);
		return NULL;
	}
	return new TCPStream(sd, &address);
}
