#ifndef CLIENT_HPP_
# define CLIENT_HPP_
# include <stdio.h>
# include <stdlib.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <netinet/tcp.h>
# include "io_event.hpp"

# include "http.hpp"
# include "listen_config.hpp"

class Client : public IOEvent
{
 public:
	Client(const int fd, ListenConfig* const conf);
	virtual ~Client(void);

	void Open(void);
	void Read(void);
	void Write(void);
	void Update(void);
 private:
	Client(void);
	Client(const Client &obj);
	Client& operator=(const Client &obj);

	void SetSocket(int fd);

	std::stringstream in_;
	std::stringstream out_;

	ListenConfig* conf_;
	Http http_;
};

# endif /* CLIENT_HPP_ */
