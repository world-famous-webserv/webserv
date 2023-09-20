#ifndef Listener_HPP
#define Listener_HPP

#include <string>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include "TcpStream.hpp"

#define MAX_CLIENTS 5
#define BUFFER_SIZE 1024

using namespace std;

class Listener {
	private:
		int		m_serverSocket;
		string	m_address;
		int		m_port;
		bool	m_listening;

		Listener() {};

	public:
		Listener(int port, const char* address);
		~Listener();

		int			start();
		TCPStream*  accept();
		int			get_sd() { return m_serverSocket; }
};

#endif // !