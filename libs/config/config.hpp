#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <map>
#include <vector>

class Config
{
    public:
        typedef std::string stringT;
        typedef stringT serverNameT, keyT, valueT;
        typedef std::map<keyT, valueT> serverT;
        typedef std::map<serverNameT, serverT> dictT;
        typedef dictT::const_iterator const_iterator;

        ~Config();
        Config();
        Config(const stringT &file);
        Config(const Config &other);
        Config &operator=(const Config &other);
        std::vector<keyT> get(const serverNameT &serverName, const keyT &key);
        dictT mDict;
    
    private:

        bool parse(const stringT &file);
        inline stringT strtrim(const stringT &str, const stringT &whitespace=" \t\n\r");
        inline bool startsWith(const stringT &str, const stringT &prefix);
        inline bool endsWith(const stringT &str, const stringT &suffix);
        inline stringT joinVector(const std::vector<keyT> &elements, const stringT& separator);
};

#endif /* CONFIG_HPP */
