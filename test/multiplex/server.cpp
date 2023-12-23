#include "server.hpp"

/* ************************************************************************** */
// Orthodox Canonical Form
/* ************************************************************************** */

Server::~Server(void)
{
}

Server::Server(ListenConfig *conf): conf_(conf)
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

	const char *node = conf_->address.c_str();
	const char *service= conf_->port.c_str();
	const int ret = getaddrinfo(node, service, &hints, info);
	if (ret != 0)
		std::cerr << node << ":" << service << ": " << gai_strerror(ret) << std::endl;
	return ret;
}

void Server::SetSocket(int fd)
{
	int on = 1;

	struct linger linger;
	linger.l_onoff = 1;
	linger.l_linger = 10;
	if (setsockopt(fd, SOL_SOCKET, SO_LINGER, &linger, sizeof(linger)) == -1)
		std::cerr << "SetSocket::SO_LINGER" << strerror(errno) << std::endl;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
		std::cerr << "SetSocket::SO_REUSEADDR" << strerror(errno) << std::endl;
	// default_server
	// ssl
	// http2
	// quic
	// proxy_protocol
#ifdef SO_SETFIB
	if (conf_->setfib != -1
		&& setsockopt(fd, SOL_SOCKET, SO_SETFIB, &conf_->setfib, sizeof(conf_->setfib)) == -1)
		std::cerr << "SetSocket::SO_SETFIB" << strerror(errno) << std::endl;
#endif
	if (conf_->fastopen != -1
		&& setsockopt(fd, IPPROTO_TCP, TCP_FASTOPEN, &conf_->fastopen, sizeof(conf_->fastopen)) == -1)
		std::cerr << "SetSocket::TCP_FASTOPEN" << strerror(errno) << std::endl;
	if (conf_->rcvbuf != -1
		&& setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &conf_->rcvbuf, sizeof(conf_->rcvbuf)) == -1)
		std::cerr << "SetSocket::SO_RCVBUF" << strerror(errno) << std::endl;
	if (conf_->sndbuf != -1
		&& setsockopt(fd, SOL_SOCKET, SO_SNDBUF, &conf_->sndbuf, sizeof(conf_->sndbuf)) == -1)
		std::cerr << "SetSocket::SO_SNDBUF" << strerror(errno) << std::endl;
#ifdef SO_ACCEPTFILTER
	if (conf_->accept_filter.length() > 0
		&& setsockopt(fd, SOL_SOCKET, SO_ACCEPTFILTER, conf_->accept.c_str(), conf_->accept_filter.length()) == -1)
		std::cerr << "SetSocket::SO_ACCEPTFILTER" << strerror(errno) << std::endl;
#endif
#ifdef TCP_DEFER_ACCEPT
	int timeout = 0;
	if (conf_->deferred == true
		&& setsockopt(fd, IPPROTO_TCP, TCP_DEFER_ACCEPT, &timeout, sizeof(timeout)) == -1)
		std::cerr << "SetSocket::TCP_DEFER_ACCEPT" << strerror(errno) << std::endl;
#endif
	if (conf_->ipv6only == true
		&& setsockopt(fd, IPPROTO_IPV6, IPV6_V6ONLY, &conf_->ipv6only, sizeof(conf_->ipv6only)) == -1)
		std::cerr << "SetSocket::IPV6_V6ONLY" << strerror(errno) << std::endl;
	if (conf_->reuseport == true
		&& setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &conf_->reuseport, sizeof(conf_->reuseport)) == -1)
		std::cerr << "SetSocket::SO_REUSEPORT" << strerror(errno) << std::endl;
#ifdef SO_KEEPALIVE
	if (conf_->so_keepalive == true
		&& setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &conf_->so_keepalive, sizeof(conf_->so_keepalive)) == -1)
		std::cerr << "SetSocket::SO_KEEPALIVE" << strerror(errno) << std::endl;
#endif
#ifdef TCP_KEEPIDLE
	if (conf_->keepidle
		&& setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, &conf_->keepidle, sizeof(conf_->keepidle)) == -1)
		std::cerr << "SetSocket::TCP_KEEPIDLE" << strerror(errno) << std::endl;
#endif
	if (conf_->keepintvl
		&& setsockopt(fd, IPPROTO_TCP, TCP_KEEPINTVL, &conf_->keepintvl, sizeof(conf_->keepintvl)) == -1)
		std::cerr << "SetSocket::TCP_KEEPINTVL" << strerror(errno) << std::endl;
	if (conf_->keepcnt
		&& setsockopt(fd, IPPROTO_TCP, TCP_KEEPCNT, &conf_->keepcnt, sizeof(conf_->keepcnt)) == -1)
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
			if (listen(fd, conf_->backlog) == 0)
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
	std::cout << "Server::Open - " << conf_->address << ":" << conf_->port << std::endl;
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
