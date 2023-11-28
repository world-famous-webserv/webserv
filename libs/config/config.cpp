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

void Config::Parse(const std::string &file)
{
    std::ifstream in(file.c_str(), std::ios_base::in);
	if (in.is_open() == false) {
        error_msg_ = "Config: File not found.";
        return;
    }
    std::ostringstream oss;
    for (std::string line; std::getline(in, line);) {
        if (line.empty() == true)
            continue;
        const size_t pos = line.find('#');
        if (pos == std::string::npos)
            oss << line << '\n';
        else if (pos) {
            // performance issue
            // oss << line.substr(0, pos) << '\n';
            line[pos] = '\0';
            oss << line << '\n';
        }
    }
	in.close();

    const std::vector<std::string> tokens = Utils::StringSplit(oss.str());

    if (Utils::CheckBrackets(tokens) == false) {
        error_msg_ = "Config: Brackets are not balanced.";
        return;
    }

    // for (std::vector<std::string>::size_type i = 0, end = tokens.size(); i < end; ++i)
    //     std::cout << tokens[i] << " ";

    size_t idx = 0;
    main_ = Block::ParseMain(tokens, idx, error_msg_);
}

bool Config::IsOpen() const
{
    return error_msg_.empty();
}

std::string Config::ErrorMsg() const
{
    return error_msg_;
}

BlockServer_t Config::GetServer(const std::string &host) const
{
    // host안에 세미콜론이 있다고 가정
    const size_t idx = host.find(':');

    const std::string name = host.substr(0, idx);
    const std::string port = host.substr(idx + 1);

    std::vector<BlockServer_t> servers;
    for (std::vector<BlockServer_t>::const_iterator i = main_.http.servers.begin(), end = main_.http.servers.end(); i != end; ++i) {
        const BlockServer_t &server = *i;

        if (std::find(server.server_name.begin(), server.server_name.end(), name) == server.server_name.end())
            continue;
        
        const std::vector<listen_t> &listens = server.listens;
        for (std::vector<listen_t>::const_iterator j = listens.begin(), end = listens.end(); j != end; ++j) {
            const listen_t &listen = *j;
            if (listen.port == port) {
                if (listen.default_server == true)
                    return server;
                servers.push_back(server);
                break;
            }
        }
    }

    // 만약 servers가 비어있다면? 무조건 하나는 있다고 가정.
    return servers.front();
}
