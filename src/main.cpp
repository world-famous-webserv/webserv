#include <cstdlib>
#include <iostream>

#if 1	// TODO: change
#include "listen_config.hpp"
#include "server.hpp"
#endif

int main(int argc, char **argv)
{
    if (argc != 1 && argc != 2)
	{
        std::cerr << "Usage: " << argv[0] << " [configuration file]" << std::endl;
        return (EXIT_FAILURE);
    }
	// 1. config
	ListenConfig config;

	// 2. service
	Multiplex::GetInstance().AddItem(new Server(&config));
	Multiplex::GetInstance().Loop();
    return (EXIT_SUCCESS);
}
