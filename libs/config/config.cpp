#include "config.hpp"

Config::~Config() { }
Config::Config(const stringT &file)
{
    if (parse(file) == false)
        exit(EXIT_FAILURE);
}

Config::Config(const Config &other)
{
    *this = other;
}

Config &Config::operator=(const Config &other)
{
    if (this == &other)
        return *this;
    return *this;
}

std::vector<Config::keyT> Config::get(const serverNameT &serverName, const keyT &key)
{
    Config::const_iterator it = mDict.begin();
    std::vector<valueT> val;

    while (it != mDict.end()) {
        if (it->first == serverName) {
            serverT server = it->second;
            if (server.find(key) != server.end())
                val.push_back(server[key]);
        }
        ++it;
    }
    if (val.size() == 0)
        throw std::runtime_error(serverName + ", " + key + " not found in the map");
    return val;
}

bool Config::parse(const stringT &file)
{
    std::ifstream in(file.c_str(), std::ios_base::in);
	if (in.is_open() == false) {
		std::cout << "Error: not a valid file." << '\n';
		return false;
	}
    
    serverT Server;
    std::vector<keyT> vKey;
    for (stringT prev, line; std::getline(in, line); ) {

        line = strtrim(line);
        std::replace(line.begin(), line.end(), '\t', ' ');
        if (line == "")
            continue;
        if (line == "}" || endsWith(line, "}")) {
            if (vKey.size() == 0) {
                mDict[Server["server_name"]] = Server;
                Server.clear();
            } else
                vKey.pop_back();
            continue;
        } else if (line == "{") {
            if (Server.size())
                vKey.push_back(prev);
            else {
                const stringT::size_type idx = prev.find(':');
                if (idx == stringT::npos)
                    Server["server_name"] = "default";
                else
                    Server["server_name"] = strtrim(prev.substr(idx + 1));
            }
            continue;
        } else if (endsWith(line, "{")) {
            if (Server.size())
                vKey.push_back(strtrim(line.substr(0, line.find('{'))));
            else {
                const stringT::size_type idx = line.find(':');
                if (idx == stringT::npos)
                    Server["server_name"] = "default";
                else
                    Server["server_name"] = strtrim(line.substr(idx + 1, line.find('{') - idx - 1));
            }
            continue;
        }
        const stringT::size_type idx = line.find(' ');
        if (idx == stringT::npos)
            return false;
        keyT key;
        if (vKey.size())
            key = joinVector(vKey, " ") + " " + strtrim(line.substr(0, idx));
        else
            key = strtrim(line.substr(0, idx));
        const valueT val = strtrim(line.substr(idx + 1), ";");
        Server[key] = val;

        prev = line;
    }
    return true;
}

inline Config::stringT Config::strtrim(const stringT &str, const stringT &whitespace) {
    const size_t start = str.find_first_not_of(whitespace);
    if (start == stringT::npos)
        return "";
    const size_t end = str.find_last_not_of(whitespace);
    return str.substr(start, end - start + 1);
}

inline bool Config::startsWith(const stringT &str, const stringT &prefix) {
    return str.compare(0, prefix.length(), prefix) == 0;
}

inline bool Config::endsWith(const stringT &str, const stringT &suffix) {
    if (str.length() < suffix.length())
        return false;
    return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
}

inline Config::stringT Config::joinVector(const std::vector<keyT> &elements, const stringT &separator) {
    stringT joined;
    
    for (std::vector<keyT>::size_type i = 0, end = elements.size(); i < end; ++i) {
        joined += elements[i];        
        if (i < elements.size() - 1)
            joined += separator;
    }
    return joined;
}
