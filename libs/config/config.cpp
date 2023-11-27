#include "config.hpp"

Config::~Config() { }

Config::Config(const std::string &file): error_msg_("")
{
    Parse(file);
}

Config::Config(const Config &obj)
{
    *this = obj;
}

Config &Config::operator=(const Config &obj)
{
    if (this == &obj)
        return *this;
    error_msg_ = obj.error_msg_;
    main_ = obj.main_;
    return *this;
}

#include <iostream>
void Config::Parse(const std::string &file)
{
    std::ifstream in(file.c_str(), std::ios_base::in);
	if (in.is_open() == false) {
        error_msg_ = "Config: File not found.";
        return;
    }
	std::string read;
    for (std::string line; std::getline(in, line);) {
        if (line.empty() == true)
            continue;
        const size_t pos = line.find('#');
        if (pos == std::string::npos)
            read += line + "\n";
        else if (pos)
            read += line.substr(0, pos) + "\n";
    }
    std::cout << read << std::endl;

	in.close();
    const std::vector<std::string> tokens = Utils::stringSplit(read);

    if (Utils::CheckBrackets(tokens) == false) {
        error_msg_ = "Config: Brackets are not balanced.";
        return;
    }


    // for (std::vector<std::string>::size_type i = 0, end = tokens.size(); i < end; ++i)
    //     std::cout << tokens[i] << " ";

    size_t idx = 0;
    main_ = Block::ParseMain(tokens, idx, error_msg_);
}

bool Config::is_open() const
{
    return error_msg_.empty();
}

std::string Config::error_msg() const
{
    return error_msg_;
}

// BlockServer_t Config::get_server(const std::string &host) const
// {
//     const size_t idx = host.find(':');

//     std::vector<BlockServer_t> servers;
//     for (std::vector<BlockServer_t>::const_iterator i = main_.http.servers.begin(), end = main_.http.servers.end(); i != end; ++i) {
//         const BlockServer_t &server = *i;

//         if (std::find(server.server_name.begin(), server.server_name.end(), name) == server.server_name.end())
//             continue;
        
//         const std::vector<listen_t> &listens = server.listens;
//         for (std::vector<listen_t>::const_iterator j = listens.begin(), end = listens.end(); j != end; ++j) {
//             const listen_t &listen = *j;
//             if (listen.port == port) {
//                 servers.push_back(server);
//                 break;
//             }
//         }
//     }
//     return servers;
// }
