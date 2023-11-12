#include "configServer.hpp"

ConfigServer::~ConfigServer() { }
ConfigServer::ConfigServer() { }
ConfigServer::ConfigServer(const ConfigServer &obj) { *this = obj; }
ConfigServer& ConfigServer::operator=(const ConfigServer &obj)
{
    if (this == &obj)
        return *this;
    return *this;
}

ConfigServer ConfigServer::parseServer(const std::vector<std::string> &tokens, std::vector<std::string>::size_type &i)
{
    std::vector<std::string>::size_type end = tokens.size();
    ConfigServer server;

    if (tokens[i] != "server")
        throw BadFileFormatException();
    ++i;
    if (i >= end || tokens[i] != "{")
        throw BadFileFormatException();
    ++i;
    while (i < end) {
        if (tokens[i] == "}") {
            ++i;
            break;
        }
        if (tokens[i] == "listen") {
            if (i + 1 >= end)
                throw BadFileFormatException();
            server.listen_ = atoi(tokens[i + 1].c_str());
            i += 2;
        }
        else if (tokens[i] == "host") {
            if (i + 1 >= end)
                throw BadFileFormatException();
            server.host_ = tokens[i + 1];
            i += 2;
        }
        else if (tokens[i] == "server_name") {
            if (i + 1 >= end)
                throw BadFileFormatException();
            server.server_name_ = tokens[i + 1];
            i += 2;
        }
        else if (tokens[i] == "root") {
            if (i + 1 >= end)
                throw BadFileFormatException();
            server.root_ = tokens[i + 1];
            i += 2;
        }
        else if (tokens[i] == "client_max_body_size") {
            if (i + 1 >= end)
                throw BadFileFormatException();
            server.client_max_body_size_ = atoi(tokens[i + 1].c_str());
            i += 2;
        }
        else if (tokens[i] == "error_page") {
            if (i + 2 >= end)
                throw BadFileFormatException();
            server.error_page_[atoi(tokens[i + 1].c_str())] = tokens[i + 2];
            i += 3;
        }
        else if (tokens[i] == "index") {
            if (i + 1 >= end)
                throw BadFileFormatException();
            ++i;
            while (i < end) {
                if (ConfigStringUtils::endsWith(tokens[i], ".html") == false)
                    break;
                server.index_.push_back(tokens[i]);
                ++i;
            }
        }
        else if (tokens[i] == "location") {
            if (i + 5 >= end || tokens[i + 2] != "{")
                throw BadFileFormatException();
            i += 3;
            server.location_[tokens[i + 1]] = ConfigLocation::parseLocation(tokens, i);
        }
        else
            throw BadFileFormatException();
    }
    return server;
}

const char*	ConfigServer::BadFileFormatException::what() const throw()
{
    return "Bad file format";
}
