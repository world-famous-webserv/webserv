#include <cstdlib>
#include <iostream>

#include "config.hpp"
#include "server.hpp"

int main(int argc, char **argv)
{
    if (argc != 1 && argc != 2)
	{
        std::cerr << "Usage: " << argv[0] << " [configuration file]" << std::endl;
        return (EXIT_FAILURE);
    }
	// 1. config
    Config config(argc == 2 ? argv[1] : "../data/default.conf");

	// 2. service
	std::vector<BlockServer_t> servers = config.GetServers();

	for (size_t i = 0; i < servers.size(); ++i) {
		const BlockServer_t &server = servers[i];
		for (size_t j = 0; j < server.listens.size(); ++j) {
			const listen_t &listen = server.listens[j];
			Multiplex::GetInstance().AddItem(new Server(server, listen));
		}
	}
	Multiplex::GetInstance().Loop();
    return (EXIT_SUCCESS);
}
