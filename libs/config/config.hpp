#ifndef LIBS_CONFIG_CONFIG_HPP_
#define LIBS_CONFIG_CONFIG_HPP_

#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <map>
#include <vector>

#include "configStringUtils.hpp"
#include "configFileReader.hpp"
#include "configServer.hpp"

class Config
{
    public:
        ~Config();
        Config();
        Config(const std::string &file);
        Config(const Config &obj);
        Config &operator=(const Config &obj);

    private:
        void parse(const std::string &file);
        std::vector<ConfigServer> servers_;
};

#endif /* LIBS_CONFIG_CONFIG_HPP_ */
