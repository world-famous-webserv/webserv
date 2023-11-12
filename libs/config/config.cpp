#include "config.hpp"

Config::~Config() { }

Config::Config()
{
    parse("configs/default.conf");
}

Config::Config(const std::string &file)
{
    parse(file);
}

Config::Config(const Config &obj)
{
    *this = obj;
}

Config &Config::operator=(const Config &obj)
{
    if (this == &obj)
        return *this;
    return *this;
}

void Config::parse(const std::string &file)
{
    std::string s = ConfigFileReader::read(file);
    std::vector<std::string> tokens = ConfigStringUtils::stringSplit(s);

    std::vector<std::string>::size_type i = 0, end = tokens.size();
    while (i < end) {
        ConfigServer server = ConfigServer::parseServer(tokens, i);
        servers_.push_back(server);
    }
}
