#include <iostream>
#include "../../libs/config/config.hpp"

int main(int argc, char *argv[])
{
    if (argc != 2)
        return EXIT_FAILURE;
    
    Config config(argv[1]);

    std::cout << config.get("server listen") << '\n';
    std::cout << config.get("server server_name") << '\n';
    std::cout << config << '\n';
    return EXIT_SUCCESS;
}