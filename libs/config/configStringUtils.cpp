#include "configStringUtils.hpp"

ConfigStringUtils::~ConfigStringUtils() { }
ConfigStringUtils::ConfigStringUtils() { }
ConfigStringUtils::ConfigStringUtils(const ConfigStringUtils &obj) { *this = obj; }
ConfigStringUtils& ConfigStringUtils::operator=(const ConfigStringUtils &obj)
{
    if (this == &obj)
        return *this;
    return *this;
}

std::string ConfigStringUtils::strtrim(const std::string &str, const std::string &whitespace) {
    const size_t start = str.find_first_not_of(whitespace);
    if (start == std::string::npos)
        return "";
    const size_t end = str.find_last_not_of(whitespace);
    return str.substr(start, end - start + 1);
}

bool ConfigStringUtils::startsWith(const std::string &str, const std::string &prefix) {
    return str.compare(0, prefix.length(), prefix) == 0;
}

bool ConfigStringUtils::endsWith(const std::string &str, const std::string &suffix) {
    if (str.length() < suffix.length())
        return false;
    return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
}

std::string ConfigStringUtils::join(const std::vector<std::string> &elements, const std::string &separator) {
    std::string joined;
    
    for (std::vector<std::string>::size_type i = 0, end = elements.size(); i < end; ++i) {
        joined += elements[i];
        if (i < elements.size() - 1)
            joined += separator;
    }
    return joined;
}

std::vector<std::string> ConfigStringUtils::stringSplit(const std::string &str, const std::string charSet)
{
	std::vector<std::string> tokens;
	std::string::size_type start, end = 0;
	while (true) {
		start = str.find_first_not_of(charSet, end);
		if (start == std::string::npos)
			break ;
		end = str.find_first_of(charSet, start);
		if (end == std::string::npos) {
			tokens.push_back(str.substr(start));
			break ;
		}
		tokens.push_back(str.substr(start, end - start));
	}
	return tokens;
}
