#pragma once
 
#include <iostream>
#include <string>
#include <unistd.h>

class ASocket
{
	public:
		virtual ~ASocket();
		ASocket();
		ASocket(const int port);
		ASocket(const ASocket & other);
		ASocket(const int port, const char * address);

		ASocket& operator = (const ASocket & other);

		virtual int handle();

	private:
		int 		mPort;
		std::string	mAddress;
		int 		mSocket;
};

class ClientSocket : public ASocket
{
	public:
		virtual ~ClientSocket();
		ClientSocket();
		ClientSocket(const int port);
		ClientSocket(const int port, const char * address);
		ClientSocket(const ClientSocket & other);

		ClientSocket & operator = (const ClientSocket * other);

		virtual int	handle();

};

class ServerSocket : public ASocket
{
	public:
		virtual ~ServerSocket();
		ServerSocket();
		ServerSocket(const int port);
		ServerSocket(const int port, const char * address);
		ServerSocket(const ServerSocket & other);

		ServerSocket & operator = (const ServerSocket * other);

		virtual int	handle();

};


