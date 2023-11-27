#ifndef SERVER_HPP_
# define SERVER_HPP_
# include <stdio.h>
# include <stdlib.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <netinet/tcp.h>
# include "io_event.hpp"

# include "listen_config.hpp"
# include "client.hpp"

class Server : public IOEvent
{
 public:
	Server(ListenConfig* conf);
	virtual ~Server(void);

	void Open(void);
	void Read(void);
	void Write(void);
	void Update(void);
 private:
	Server(void);
	Server(const Server &obj);
	Server& operator=(const Server &obj);

	void SetSocket(int fd);
	int	GetAddrInfo(struct addrinfo **info);
	int CreatSocket(struct addrinfo *info);

	ListenConfig* conf_;
};

# endif /* SERVER_HPP_ */
