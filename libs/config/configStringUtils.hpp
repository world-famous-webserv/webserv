#ifndef LIBS_CONFIG_CONFIGSTRINGUTILS_HPP_
#define LIBS_CONFIG_CONFIGSTRINGUTILS_HPP_

#include <string>
#include <vector>
#include <iostream>

class ConfigStringUtils
{
    public:
        ~ConfigStringUtils();
        ConfigStringUtils();
        ConfigStringUtils(const ConfigStringUtils &obj);
        ConfigStringUtils& operator=(const ConfigStringUtils &obj);

        static std::string strtrim(const std::string &str, const std::string &whitespace=" \t\n\r");
        static bool startsWith(const std::string &str, const std::string &prefix);
        static bool endsWith(const std::string &str, const std::string &suffix);
        static std::string join(const std::vector<std::string> &elements, const std::string& separator);
		static std::vector<std::string> stringSplit(const std::string &str, const std::string charSet=" \t\n\r");
};

#endif /* LIBS_CONFIG_CONFIGSTRINGUTILS_HPP_ */
