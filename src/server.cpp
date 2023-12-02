#include "server.hpp"

/* ************************************************************************** */
// Orthodox Canonical Form
/* ************************************************************************** */

Server::~Server(void)
{
	this->Close();
}

Server::Server(const Conf &conf):
	conf_(conf)
{
	this->Open();
}

/* ************************************************************************** */
// Utility
/* ************************************************************************** */

int	Server::GetAddrInfo(struct addrinfo **info)
{
	struct addrinfo hints;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_addrlen = 0;
	hints.ai_addr = NULL;
	hints.ai_canonname = NULL;
	hints.ai_next = NULL;

	const char *node = conf_.listen.address.c_str();
	const char *service= conf_.listen.port.c_str();
	const int ret = getaddrinfo(node, service, &hints, info);
	if (ret != 0)
		std::cerr << node << ":" << service << ": " << gai_strerror(ret) << std::endl;
	return ret;
}

void Server::SetSocket(int fd)
{
	int on = 1;

	if (setsockopt(fd, SOL_SOCKET, SO_LINGER, &conf_.linger, sizeof(linger)) == -1)
		std::cerr << "SetSocket::SO_LINGER" << strerror(errno) << std::endl;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
		std::cerr << "SetSocket::SO_REUSEADDR" << strerror(errno) << std::endl;
	// default_server
	// ssl
	// http2
	// quic
	// proxy_protocol
#ifdef SO_SETFIB
	if (conf_.listen.setfib != -1
		&& setsockopt(fd, SOL_SOCKET, SO_SETFIB, &conf_.listen.setfib, sizeof(conf_.listen.setfib)) == -1)
		std::cerr << "SetSocket::SO_SETFIB" << strerror(errno) << std::endl;
#endif
	if (conf_.listen.fastopen != -1
		&& setsockopt(fd, IPPROTO_TCP, TCP_FASTOPEN, &conf_.listen.fastopen, sizeof(conf_.listen.fastopen)) == -1)
		std::cerr << "SetSocket::TCP_FASTOPEN" << strerror(errno) << std::endl;
	if (conf_.listen.rcvbuf != -1
		&& setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &conf_.listen.rcvbuf, sizeof(conf_.listen.rcvbuf)) == -1)
		std::cerr << "SetSocket::SO_RCVBUF" << strerror(errno) << std::endl;
	if (conf_.listen.sndbuf != -1
		&& setsockopt(fd, SOL_SOCKET, SO_SNDBUF, &conf_.listen.sndbuf, sizeof(conf_.listen.sndbuf)) == -1)
		std::cerr << "SetSocket::SO_SNDBUF" << strerror(errno) << std::endl;
#ifdef SO_ACCEPTFILTER
	if (conf_.listen.accept_filter.length() > 0
		&& setsockopt(fd, SOL_SOCKET, SO_ACCEPTFILTER, conf_.listen.accept_filter.c_str(), conf_.listen.accept_filter.length()) == -1)
		std::cerr << "SetSocket::SO_ACCEPTFILTER" << strerror(errno) << std::endl;
#endif
#ifdef TCP_DEFER_ACCEPT
	int timeout = 0;
	if (conf_.listen.deferred == true
		&& setsockopt(fd, IPPROTO_TCP, TCP_DEFER_ACCEPT, &timeout, sizeof(timeout)) == -1)
		std::cerr << "SetSocket::TCP_DEFER_ACCEPT" << strerror(errno) << std::endl;
#endif
	if (conf_.listen.ipv6only == true
		&& setsockopt(fd, IPPROTO_IPV6, IPV6_V6ONLY, &conf_.listen.ipv6only, sizeof(conf_.listen.ipv6only)) == -1)
		std::cerr << "SetSocket::IPV6_V6ONLY" << strerror(errno) << std::endl;
	if (conf_.listen.reuseport == true
		&& setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &conf_.listen.reuseport, sizeof(conf_.listen.reuseport)) == -1)
		std::cerr << "SetSocket::SO_REUSEPORT" << strerror(errno) << std::endl;
#ifdef SO_KEEPALIVE
	if (conf_.listen.so_keepalive == true
		&& setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &conf_.listen.so_keepalive, sizeof(conf_.listen.so_keepalive)) == -1)
		std::cerr << "SetSocket::SO_KEEPALIVE" << strerror(errno) << std::endl;
#endif
#ifdef TCP_KEEPIDLE
	if (conf_.listen.keepidle != -1
		&& setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, &conf_.listen.keepidle, sizeof(conf_.listen.keepidle)) == -1)
		std::cerr << "SetSocket::TCP_KEEPIDLE" << strerror(errno) << std::endl;
#endif
	if (conf_.listen.keepintvl != -1
		&& setsockopt(fd, IPPROTO_TCP, TCP_KEEPINTVL, &conf_.listen.keepintvl, sizeof(conf_.listen.keepintvl)) == -1)
		std::cerr << "SetSocket::TCP_KEEPINTVL" << strerror(errno) << std::endl;
	if (conf_.listen.keepcnt != -1
		&& setsockopt(fd, IPPROTO_TCP, TCP_KEEPCNT, &conf_.listen.keepcnt, sizeof(conf_.listen.keepcnt)) == -1)
		std::cerr << "SetSocket::TCP_KEEPCNT" << strerror(errno) << std::endl;
}

int Server::CreatSocket(struct addrinfo *info)
{
	int fd = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
	if (fd != -1)
	{
		this->SetSocket(fd);
		if (bind(fd, info->ai_addr, info->ai_addrlen) == 0)
		{
			if (listen(fd, conf_.listen.backlog) == 0)
				return fd;
		}
		close(fd);
	}
	return -1;
}

/* ************************************************************************** */
// Main
/* ************************************************************************** */

void Server::Open(void)
{
	std::cout << "Server::Open - " << conf_.listen.address << ":" << conf_.listen.port << std::endl;
	struct addrinfo *info = NULL;
	if (this->GetAddrInfo(&info) != 0)
		return ;

	for (struct addrinfo *curr = info; curr != NULL; curr = curr->ai_next) {
		identifier_ = this->CreatSocket(curr);
		if (identifier_ != -1) break;
	}
	if (identifier_ == -1)
		std::cerr << "Unkown Host" << std::endl;
	else
		std::cout << "Listening..." << std::endl;
	freeaddrinfo(info);
}

void Server::Read(void)
{
	if (identifier_ == -1)
		return ;
	std::cout << "Server::Read" << std::endl;
	struct sockaddr_in info;
	socklen_t len = sizeof(info);
	int fd = accept(identifier_, reinterpret_cast<struct sockaddr*>(&info), &len);
	if (fd == -1)
	{
		std::cerr << "- connection error: " << fd << std::endl;
		return ;
	}
	std::cout << "- connection success: " << fd << std::endl;
	this->SetSocket(fd);
	Multiplex::GetInstance().AddItem(new Client(fd, conf_));
}

void Server::Write(void)
{
	if (identifier_ == -1)
		return ;
	std::cout << "Server::Write" << std::endl;
}

void Server::Update(void)
{
	if (identifier_ == -1)
		return ;
}
