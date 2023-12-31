#include "server.hpp"

/* ************************************************************************** */
// Orthodox Canonical Form
/* ************************************************************************** */

Client::~Client(void)
{
}

Client::Client(const int fd, ListenConfig *const conf): conf_(conf)
{
	identifier_ = fd;
	this->Open();
}

/* ************************************************************************** */
// Utility
/* ************************************************************************** */

void Client::SetSocket(int fd)
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

/* ************************************************************************** */
// Main
/* ************************************************************************** */

void Client::Open(void)
{
	std::cout << "Client::Open" << std::endl;
	this->SetSocket(identifier_);
}

void Client::Read(void)
{
	if (identifier_ == -1)
		return ;
	char buf[1025] = {0};
	const ssize_t nbytes = recv(identifier_, buf, sizeof(buf) - 1, 0);
	if (nbytes < 0) {
		std::cerr << "Client::Read fail: " << strerror(errno) << std::endl;
		return this->Close();
	}
	else if (nbytes > 0)
	{
		buf[nbytes] = '\0';
		sendbuf_.append(buf);
		std::cout << "Client::Read #########################Start" << std::endl;
		std::cout << buf << std::endl;
		std::cout << "Client::Read ###########################End" << std::endl;
	}
}

void Client::Write(void)
{
	if (identifier_ == -1)
		return ;
	const size_t len = sendbuf_.length();
	if (len == 0)
		return ;
	const ssize_t nbytes = send(identifier_, sendbuf_.c_str(), len, 0);
	if (nbytes < 0) {
		std::cerr << "Client::Write fail: " << strerror(errno) << std::endl;
		return this->Close();
	}
	else
	{
		std::cout << "Client::Write ########################Start" << std::endl;
		std::cout << sendbuf_.substr(0, nbytes) << std::endl;
		std::cout << "Client::Write ##########################End" << std::endl;
		sendbuf_.erase(0, nbytes);
	}
}

void Client::Update(void)
{
	if (identifier_ == -1)
		return ;
}