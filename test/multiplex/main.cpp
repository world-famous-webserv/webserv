#include "server.hpp"

static void setConfig(ListenConfig& config)
{
//	config.address = "*";		// bsd error, linux ok
	config.address = "127.0.0.1";
	config.port = "8080";
	config.default_server = true;
	config.ssl = false;
	config.http2 = false;
	config.quic = false;
	config.proxy_protocol = false;
	config.setfib = -1;			// SO_SETFIB
	config.fastopen = -1;			// TCP_FASTOPEN
	config.backlog = -1;			// or 511
	config.rcvbuf = -1;			// SO_RCVBUF
	config.sndbuf = -1;			// SO_SNDBUF
	config.accept_filter = "";		// SO_ACCEPTFILTER (dataready, httpready)
	config.deferred = false;		// TCP_DEFER_ACCEPT
	config.bind = true;
	config.ipv6only = false;		// IPV6_V6ONLY
	config.reuseport = false;		// SO_REUSEPORT
	config.so_keepalive = 1;	// SO_KEEPALIVE
	config.keepidle = 0;			// TCP_KEEPIDLE
	config.keepintvl = 0;			// TCP_KEEPINTVL
	config.keepcnt = 0;			// TCP_KEEPCNT
}

int	main(void)
{
	ListenConfig config;
	setConfig(config);
	Multiplex::GetInstance().AddItem(new Server(&config));
	Multiplex::GetInstance().Loop();
    return 0;
}
