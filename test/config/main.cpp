#include <iostream>
#include <vector>
#include "config.hpp"

int main(int argc, char *argv[])
{
    if (argc != 2)
        return EXIT_FAILURE;
    
    Config config(argv[1]);

    // localhost라는 이름을 가진 server의 listen 값을 vector에 담아 반환
    // localhost라는 이름을 가진 server가 여러개일 경우를 고려하여 설계
    const Config::serverNameT serverName = "localhost";
    const Config::keyT key = "listen";

    const std::vector<Config::valueT> v = config.get(serverName, key);
    std::vector<Config::valueT>::const_iterator it = v.begin();

    while (it != v.end()) {
        std::cout << serverName << " = " << *it << '\n';
        ++it;
    }
    return EXIT_SUCCESS;
}