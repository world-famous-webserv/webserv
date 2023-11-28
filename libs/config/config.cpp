#include "config.hpp"

Config::~Config() { }
Config::Config(const std::string &file):
    error_msg_("")
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
        else if (pos)
            oss << line.substr(0, pos) << '\n';
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

std::string Config::GetUrl(const std::string &str) const
{
    std::vector<std::string> parts;
    std::istringstream iss(str);
    for (std::string part; std::getline(iss, part, '/');) {
		if (part == "" || part == ".")
            continue;
		if (part == "..") {
			if (parts.empty())
				return "";
			parts.pop_back();
		}
        else
			parts.push_back(part);
	}
    if (parts.empty())
        return "";
    std::string url;
    for (std::vector<std::string>::const_iterator i = parts.begin(), end = parts.end(); i != end; ++i)
        url += "/" + *i;
    return url;
}

std::string Config::GetPath(const std::string &url) const
{
    const BlockLocation_t &location = GetLocation(url);
    std::string path = location.root + url.substr(location.name.length() - (location.name[0] == '/'));
    return path;
}

BlockLocation_t Config::GetLocation(const std::string &url) const
{
    BlockLocation_t *ret = NULL;
    size_t max_location_name_length = 0;

    std::vector<BlockServer_t> servers = GetServers();
    for (size_t i = 0, end = servers.size(); i != end; ++i) {
        BlockServer_t &server = servers[i];
        std::vector<BlockLocation_t> &locations = server.locations;
        for (size_t j = 0, end = locations.size(); j != end; ++j) {
            BlockLocation_t &location = locations[j];
            if (url.length() > max_location_name_length && url.length() >= location.name.length() && url.compare(0, location.name.length(), location.name) == 0) {
                max_location_name_length = location.name.length();
                ret = &location;
            }
        }
    }
    return *ret;
}
