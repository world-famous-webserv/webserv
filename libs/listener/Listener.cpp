#include "Listener.hpp"

Listener::Listener(int port, const char* address)
	: m_serverSocket(0), m_port(port), m_address(address), m_listening(false) {}

Listener::~Listener() {
	if (m_serverSocket > 0)
		close(m_serverSocket);
}

int Listener::start() {
	if (m_listening == true)
		return 0;

	m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(m_port);
	address.sin_addr.s_addr = INADDR_ANY;

	//int optval = 1;
	//setsockopt(m_serverSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

	int result = bind(m_serverSocket, reinterpret_cast<const struct sockaddr*>(&address), sizeof(address));

	if (result != 0) {
		perror("bind() failed");
		return result;
	}
	result = listen(m_serverSocket, MAX_CLIENTS);
	if (result != 0) {
		perror("listen() failed");
		return result;
	}
	m_listening = true;
	return result;
}

TCPStream* Listener::accept() {
	if (m_listening == false) 
		return NULL;

	struct sockaddr_in address;
	socklen_t len = sizeof(address);
	memset(&address, 0, sizeof(address));
	
	int sd = ::accept(m_serverSocket, reinterpret_cast<struct sockaddr*>(&address), &len);

	if (sd < 0) {
		perror("accept() failed");
		return NULL;
	}
	return new TCPStream(sd, &address);
}