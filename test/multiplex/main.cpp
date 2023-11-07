#include "server.hpp"

int	main(void)
{
	ListenConfig config = {
	//	.address = "*",			// bsd error, linux ok
		.address = "127.0.0.1",
		.port = "8080",
		.default_server = true,
		.ssl = false,
		.http2 = false,
		.quic = false,
		.proxy_protocol = false,
		.setfib = -1,			// SO_SETFIB
		.fastopen = -1,			// TCP_FASTOPEN
		.backlog = -1,			// or 511
		.rcvbuf = -1,			// SO_RCVBUF
		.sndbuf = -1,			// SO_SNDBUF
		.accept_filter = "",	// SO_ACCEPTFILTER (dataready, httpready)
		.deferred = false,		// TCP_DEFER_ACCEPT
		.bind = true,
		.ipv6only = false,		// IPV6_V6ONLY
		.reuseport = false,		// SO_REUSEPORT
		.so_keepalive = 1,		// SO_KEEPALIVE
		.keepidle = 0,			// TCP_KEEPIDLE
		.keepintvl = 0,			// TCP_KEEPINTVL
		.keepcnt = 0			// TCP_KEEPCNT
	};
	Multiplex::GetInstance().AddItem(new Server(&config));
	Multiplex::GetInstance().Loop();
    return 0;
}
