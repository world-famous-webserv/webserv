#include "TcpStream.hpp"

TCPStream::TCPStream(int sd, struct sockaddr_in* address) : m_clientSocket(sd) {
	char ip[50];
	// 사용불가함수
	inet_ntop(PF_INET, reinterpret_cast<const struct in_addr*>(&address->sin_addr.s_addr), ip, sizeof(ip)-1);
	m_peerIP = ip;
	m_peerPort = ntohs(address->sin_port);
}

TCPStream::~TCPStream() {
	close(m_clientSocket);
}

ssize_t	TCPStream::send(char* buffer, size_t len) {
	return write(m_clientSocket, buffer, len);
}

ssize_t	TCPStream::receive(char* buffer, size_t len) {
	return read(m_clientSocket, buffer, len);
}
