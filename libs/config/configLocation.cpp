#include "configLocation.hpp"

ConfigLocation::~ConfigLocation() { }
ConfigLocation::ConfigLocation() { }
ConfigLocation::ConfigLocation(const ConfigLocation &obj) { *this = obj; }
ConfigLocation& ConfigLocation::operator=(const ConfigLocation &obj)
{
    if (this == &obj)
        return *this;
    return *this;
}

ConfigLocation ConfigLocation::parseLocation(const std::vector<std::string> &tokens, std::vector<std::string>::size_type &i)
{
    std::vector<std::string>::size_type end = tokens.size();
    ConfigLocation location;

    std::set<std::string> methods;
    methods.insert("GET"); methods.insert("HEAD"); methods.insert("POST");
    methods.insert("PUT"); methods.insert("DELETE"); methods.insert("CONNECT");
    methods.insert("OPTIONS"); methods.insert("TRACE"); methods.insert("PATCH");

    while (i < end) {
        if (tokens[i] == "}") {
            ++i;
            break;
        }
        if (tokens[i] == "root") {
            if (i + 1 >= end)
                throw BadFileFormatException();
            location.root_ = tokens[i + 1];
            i += 2;
        }
        else if (tokens[i] == "cgi_path") {
            if (i + 1 >= end)
                throw BadFileFormatException();
            location.cgi_path_ = tokens[i + 1];
            i += 2;
        }
        else if (tokens[i] == "return") {
            if (i + 1 >= end)
                throw BadFileFormatException();
            location.return_ = tokens[i + 1];
            i += 2;
        }
        else if (tokens[i] == "allow_methods") {
            if (i + 1 >= end)
                throw BadFileFormatException();
            ++i;
            while (i < end) {
                if (methods.find(tokens[i]) == methods.end())
                    break ;
                location.allowed_methods_.insert(tokens[i]);
                ++i;
            }
        }
        else if (tokens[i] == "autoindex") {
            if (i + 1 >= end)
                throw BadFileFormatException();
            location.autoindex_ = (tokens[i + 1] == "on");
            i += 2;
        }
        else if (tokens[i] == "index") {
            if (i + 1 >= end)
                throw BadFileFormatException();
            ++i;
            while (i < end) {
                if (tokens[i].find('.') == false)
                    break;
                location.index_.push_back(tokens[i]);
                ++i;
            }
        }
        else {
            std::cout << tokens[i] << std::endl;
            throw UnknownDirectiveException();
        }
    }
    return location;
}

const char* ConfigLocation::BadFileFormatException::what() const throw()
{
    return "Bad file format";
}

const char* ConfigLocation::UnknownDirectiveException::what() const throw()
{
    return "Unknown directive";
}
