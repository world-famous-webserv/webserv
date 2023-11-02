#include "config.hpp"

Config::~Config() { }
Config::Config(const std::string file)
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

std::vector<std::string> Config::get(const std::string serverName, const std::string key)
{
    Config::const_iterator it = mDict.begin();
    std::vector<std::string> val;

    while (it != mDict.end()) {
        if (it->first == serverName) {
            value_type server = it->second;
            if (server.find(key) != server.end())
                val.push_back(server[key]);
        }
        ++it;
    }
    if (val.size() == 0)
        throw std::runtime_error(serverName + ", " + key + " not found in the map");
    return val;
}

bool Config::parse(const std::string file)
{
    std::ifstream in(file, std::ios_base::in);
	if (in.is_open() == false) {
		std::cout << "Error: not a valid file." << '\n';
		return false;
	}
    
    value_type vServer;
    std::vector<std::string> vKey;
    for (std::string prev, line; std::getline(in, line); ) {

        line = strtrim(line);
        std::replace(line.begin(), line.end(), '\t', ' ');
        if (line == "")
            continue;
        if (line == "}" || endsWith(line, "}")) {
            if (vKey.size() == 0) {
                mDict[vServer["server_name"]] = vServer;
                vServer.clear();
            } else
                vKey.pop_back();
            continue;
        } else if (line == "{") {
            if (vServer.size())
                vKey.push_back(prev);
            else {
                std::string::size_type idx = prev.find(':');
                if (idx == std::string::npos)
                    vServer["server_name"] = "default";
                else
                    vServer["server_name"] = strtrim(prev.substr(idx + 1));
            }
            continue;
        } else if (endsWith(line, "{")) {
            if (vServer.size())
                vKey.push_back(strtrim(line.substr(0, line.find('{'))));
            else {
                std::string::size_type idx = line.find(':');
                if (idx == std::string::npos)
                    vServer["server_name"] = "default";
                else
                    vServer["server_name"] = strtrim(line.substr(idx + 1, line.find('{') - idx - 1));
            }
            continue;
        }
        std::string::size_type idx = line.find(' ');
        if (idx == std::string::npos)
            return false;
        std::string key;
        if (vKey.size())
            key = joinVector(vKey, " ") + " " + strtrim(line.substr(0, idx));
        else
            key = strtrim(line.substr(0, idx));
        std::string val = strtrim(line.substr(idx + 1), ";");
        vServer[key] = val;

        prev = line;
    }

    return true;
}

inline std::string Config::strtrim(const std::string& str, const std::string& whitespace) {
    size_t start = str.find_first_not_of(whitespace);
    size_t end = str.find_last_not_of(whitespace);

    if (start == std::string::npos)
        return "";
    return str.substr(start, end - start + 1);
}

inline bool Config::startsWith(const std::string& str, const std::string& prefix) {
    return str.compare(0, prefix.length(), prefix) == 0;
}

inline bool Config::endsWith(const std::string& str, const std::string& suffix) {
    if (str.length() < suffix.length()) {
        return false;
    }
    return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
}

inline std::string Config::joinVector(const std::vector<std::string>& elements, const std::string& separator) {
    std::string result;
    
    for (size_t i = 0; i < elements.size(); ++i) {
        result += elements[i];
        
        if (i < elements.size() - 1) {
            result += separator;
        }
    }
    return result;
}

