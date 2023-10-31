#include "config.hpp"

Config::~Config() { }
Config::Config(const std::string file)
{
    if (parse(file) == false)
        exit(EXIT_FAILURE);
}

Config::Config(const Config &other)
{
    (void)other;
}

Config &Config::operator=(const Config &other)
{
    if (this == &other)
        return *this;
    return *this;
}

std::string Config::get(const std::string key)
{
    std::unordered_map<std::string, std::string>::iterator it = mDict.find(key);

    if (it == mDict.end())
        throw std::runtime_error(key + " not found in the map");
    
    return it->second;
}

bool Config::parse(const std::string file)
{
    std::ifstream in(file, std::ios_base::in);
	if (in.is_open() == false) {
		std::cout << "Error: not a valid file." << '\n';
		return false;
	}
    
    std::vector<std::string> vKey;
    for (std::string prev, line; std::getline(in, line); ) {

        line = strtrim(line);
        std::replace(line.begin(), line.end(), '\t', ' ');
        if (line == "")
            continue;
        if (line == "}" || endsWith(line, "}")) {
            vKey.pop_back();
            continue;
        } else if (line == "{") {
            vKey.push_back(prev);
            continue;
        } else if (endsWith(line, "{")) {
            vKey.push_back(strtrim(line.substr(0, line.find('{'))));
            continue;
        }
        std::string::size_type idx = line.find(' ');
        if (idx == std::string::npos)
            return false;
        std::string key = joinVector(vKey, " ") + " " + strtrim(line.substr(0, idx));
        std::string val = strtrim(line.substr(idx + 1), ";");
        mDict[key] = val;
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

std::ostream &operator<<(std::ostream &stream, Config &config)
{
    std::unordered_map<std::string, std::string>::const_iterator it = config.mDict.begin();

    while (it != config.mDict.end()) {
        stream << it->first << " = " << it->second << '\n';
        ++it;
    }
    return stream;
}