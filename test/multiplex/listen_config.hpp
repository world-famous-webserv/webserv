#pragma once
#ifndef LISTEN_CONFIG_HPP_
# define LISTEN_CONFIG_HPP_

struct ListenConfig {
	std::string address;
	std::string port;
	bool default_server;
	bool ssl;
	bool http2;
	bool quic;
	bool proxy_protocol;
	int setfib;
	int fastopen;
	int backlog;
	int rcvbuf;
	int sndbuf;
	std::string accept_filter;
	bool deferred;
	bool bind;
	bool ipv6only;
	bool reuseport;
	int so_keepalive;
	int keepidle;
	int keepintvl;
	int keepcnt;
};

# endif /* LISTEN_CONFIG_HPP_ */
