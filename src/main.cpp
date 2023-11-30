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
    Config config(argc == 2 ? argv[1] : "data/default.conf");
	if (config.IsOpen() == false) {
		std::cerr << "Error: " << config.ErrorMsg() << std::endl;
		return (EXIT_FAILURE);
	}
	// 2. service
	std::vector<BlockServer_t> servers = config.GetServers();

	for (size_t i = 0; i < servers.size(); ++i) {
		for (size_t j = 0; j < servers[i].listens.size(); ++j) {
			server_t server = {&servers[i], &servers[i].listens[j]};
			Multiplex::GetInstance().AddItem(new Server(server));
		}
	}
	Multiplex::GetInstance().Loop();
    return (EXIT_SUCCESS);
}
