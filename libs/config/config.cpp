#include "config.hpp"

Config::~Config() { }
Config::Config(const std::string &file):
    error_msg_("")
{
    Parse(file);
    CheckDuplicatedPort();
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

    const std::vector<std::string> tokens = Config::StringSplit(oss.str());

    if (Config::CheckBrackets(tokens) == false) {
        error_msg_ = "Config: Brackets are not balanced.";
        return;
    }

    size_t idx = 0;
    main_ = Block::ParseMain(tokens, idx, error_msg_);
}

void Config::CheckDuplicatedPort()
{
    std::set<std::string> ports;
    const std::vector<server_t> servers = GetServers();
    for (size_t i = 0; i < servers.size(); ++i) {
        const std::vector<listen_t> &listens = servers[i].listens;
        for (size_t j = 0; j < listens.size(); ++j) {
            const listen_t &listen = listens[j];
            if (ports.find(listen.port) != ports.end() && listen.reuseport == 0) {
                error_msg_ = "Config: Port [ " + listen.port + " ] is duplicated.";
                return;
            }
            ports.insert(listen.port);
        }
    }
}

bool Config::IsOpen() const
{
    return error_msg_.empty();
}

std::string Config::ErrorMsg() const
{
    return error_msg_;
}

server_t Config::GetServer(const std::string &host) const
{
    // host안에 세미콜론이 있다고 가정
    const size_t idx = host.find(':');

    const std::string name = host.substr(0, idx);
    const std::string port = host.substr(idx + 1);

    std::vector<server_t> servers;
    for (std::vector<server_t>::const_iterator i = main_.http.servers.begin(), end = main_.http.servers.end(); i != end; ++i) {
        const server_t &server = *i;

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

std::vector<std::string> Config::StringSplit(const std::string &str)
{
	std::vector<std::string> tokens;
	std::string::size_type start, end = 0;
	while (true) {
		start = str.find_first_not_of(" \f\n\r\t\v", end);
		if (start == std::string::npos)
			break;
		end = str.find_first_of(" \f\n\r\t\v", start);
		if (end == std::string::npos) {
			tokens.push_back(str.substr(start));
			break;
		}
        int s = start, e = end;
        while (s < e) {
            if (str[s] == '{')      tokens.push_back("{");
            else if (str[s] == '}') tokens.push_back("}");
            else if (str[s] == ';') tokens.push_back(";");
            else
                break;
            ++s;
        }
        std::vector<std::string> tmp;
        while (s < e) {
            if (str[e - 1] == '{')      tmp.push_back("{");
            else if (str[e - 1] == '}') tmp.push_back("}");
            else if (str[e - 1] == ';') tmp.push_back(";");
            else
                break;
            --e;
        }
        if (e > s)
            tokens.push_back(str.substr(s, e - s));
        for (std::vector<std::string>::reverse_iterator it = tmp.rbegin(); it != tmp.rend(); ++it)
            tokens.push_back(*it);
	}
	return tokens;
}

bool Config::CheckBrackets(const std::vector<std::string> &tokens)
{
    int brackets = 0;
    for (std::vector<std::string>::const_iterator it = tokens.begin(); it != tokens.end(); ++it) {
        if (*it == "{")      ++brackets;
        else if (*it == "}") --brackets;
        if (brackets < 0)
            return false;
    }
    return brackets == 0;
}
