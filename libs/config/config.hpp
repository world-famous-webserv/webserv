#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>

class Config
{
    public:
        typedef std::unordered_map<std::string, std::string> value_type;
        typedef std::unordered_map<std::string, value_type>::const_iterator const_iterator;

        ~Config();
        Config(const std::string file);
        Config(const Config &other);
        Config &operator=(const Config &other);
        std::vector<std::string> get(const std::string serverName, const std::string key);
        std::unordered_map<std::string, value_type> mDict;
    
    private:

        Config();
        bool parse(const std::string file);

        std::string strtrim(const std::string& str, const std::string& whitespace=" \t\n\r");
        inline bool startsWith(const std::string& fullString, const std::string& prefix);
        inline bool endsWith(const std::string& str, const std::string& suffix);
        inline std::string joinVector(const std::vector<std::string>& elements, const std::string& separator);
};
