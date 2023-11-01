#include <cstdlib>
#include "ServerSocket.hpp"
#include "ClientSocket.hpp"

int main(int ac, char **av)
{
	ServerSocket*	serverSocket;

	if (ac < 2 || ac > 4) {
		printf("usage: server <port> [<ip>]\n");
		exit(1);
	}
	if (ac != 3)
		serverSocket = new ServerSocket(std::atoi(av[1]));
	else
		serverSocket = new ServerSocket(std::atoi(av[1]), av[2]);
	try {
		serverSocket->setSocket();
		while (true)
			serverSocket->accept();
	}
	catch(const std::exception & e) {
		std::cerr << "Error: " << e.what() << '\n';
	}
}
