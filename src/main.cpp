#include "sample.hpp"
#include <cstdlib>

int main(int argc, char **argv)
{
    if (argc != 1 && argc != 2)
	{
        std::cerr << "Usage: " << argv[0] << " [configuration file]" << std::endl;
        return (EXIT_FAILURE);
    }
    if (argc == 1)
        sample();
    else
        std::cout << "TODO: read [" << argv[1] << "]" << std::endl;
    return (EXIT_SUCCESS);
}
