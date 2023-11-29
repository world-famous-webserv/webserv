#ifndef SRC_CLIENT_HPP_
# define SRC_CLIENT_HPP_
# include <stdio.h>
# include <stdlib.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <netinet/tcp.h>
# include "io_event.hpp"

# include "http/http.hpp"
# include "config.hpp"

class Client : public IOEvent
{
 public:
	Client(const int fd, const BlockServer_t &server, const listen_t &listen);
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
	
	std::string GetUrl(const std::string &str) const;
	std::string GetPath(const std::string &url) const;
	const BlockLocation_t &GetLocation(const std::string &url) const;

	std::stringstream in_;
	std::stringstream out_;

	Http http_;
	BlockServer_t server_;
	listen_t listen_;
};

# endif /* SRC_CLIENT_HPP_ */
