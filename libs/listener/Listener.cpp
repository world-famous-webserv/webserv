#include "Listener.hpp"

Listener::Listener(int port, const char* address)
	: mServerSocket(0), mAddress(address), mPort(port), mListening(false) {}

Listener::Listener(int port)
	: mServerSocket(0), mAddress(""), mPort(port), mListening(false) {}

Listener::~Listener() {
	if (mServerSocket > 0)
		close(mServerSocket);
}

int Listener::start() {
	if (mListening == true)
		return 0;

	mServerSocket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in address;
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(mPort);

	address.sin_addr.s_addr = INADDR_ANY;

	int optval = 1;
	setsockopt(mServerSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

	int result = bind(mServerSocket, reinterpret_cast<const struct sockaddr*>(&address), sizeof(address));

	if (result != 0) {
		perror("bind() failed");
		return result;
	}
	result = listen(mServerSocket, MAX_CLIENTS);
	if (result != 0) {
		perror("listen() failed");
		return result;
	}
	mListening = true;
	return result;
}

TCPStream* Listener::accept() {
	if (mListening == false) 
		return NULL;

	struct sockaddr_in address;
	socklen_t len = sizeof(address);
	memset(&address, 0, sizeof(address));
	
	int sd = ::accept(mServerSocket, reinterpret_cast<struct sockaddr*>(&address), &len);

	if (sd < 0) {
		perror("accept() failed");
		return NULL;
	}
	return new TCPStream(sd, &address);
}