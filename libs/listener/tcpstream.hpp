#ifndef TCPSTREAM_HPP
#define TCPSTREAM_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>

class TCPStream {
	private:
		int			m_clientSocket;
		std::string	m_peerIP;
		int			m_peerPort;

		TCPStream(int sd, struct sockaddr_in* address);
		TCPStream();
		TCPStream(const TCPStream& stream);

	public:
		friend class Listener;

		~TCPStream();

		ssize_t 	send(char* buffer, size_t len);
		ssize_t 	receive(char* buffer, size_t len);
		int			get_sd() { return m_clientSocket; }
};

#endif