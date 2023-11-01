#include "ClientSocket.hpp"

ClientSocket::ClientSocket(const int sd,  struct sockaddr_in * address)
	:
	ASocket(-1, "")
{
	mSocket = sd;
	mPort = ntohs(address->sin_port);
	std::cout << "ClientSocket.cpp = Client->Port: " << mPort << "\n";
}

ssize_t	ClientSocket::send(char* buffer, size_t len)
{
	return write(mSocket, buffer, len);
}

ssize_t	ClientSocket::receive(char* buffer, size_t len)
{
	return read(mSocket, buffer, len);
}

// 클래스 효율적인 관리방법 찾아보기
// main문을 최대한 간결하게 만들 방안
// e.what() 함수 조절하기
