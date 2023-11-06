#pragma once
#ifndef CLIENT_HPP_
# define CLIENT_HPP_
# include <stdio.h>
# include <stdlib.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <netinet/tcp.h>
# include "io_event.hpp"

# include "listen_config.hpp"

class Client : public IOEvent
{
 public:
	Client(int fd, ListenConfig* conf);
	virtual ~Client(void);

	void Open(void);
	void Read(void);
	void Write(void);
 private:
	Client(void);
	Client(const Client &obj);
	Client& operator=(const Client &obj);

	void SetSocket(int fd);

	std::string sendbuf_;
	ListenConfig* conf_;
};

# endif /* CLIENT_HPP_ */
