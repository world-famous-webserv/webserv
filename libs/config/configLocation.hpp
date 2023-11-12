#ifndef LIBS_CONFIG_CONFIGLOCATION_HPP_
#define LIBS_CONFIG_CONFIGLOCATION_HPP_

#include <string>
#include <vector>
#include <set>
#include <exception>
#include <iostream>

class ConfigLocation {
    public:
        ~ConfigLocation();
        ConfigLocation();
        ConfigLocation(const ConfigLocation &obj);
        ConfigLocation& operator=(const ConfigLocation &obj);
        static ConfigLocation parseLocation(const std::vector<std::string> &tokens, std::vector<std::string>::size_type &i);

        class BadFileFormatException: public std::exception
        {
            public:
                virtual const char* what() const throw();
        };

        class UnknownDirectiveException: public std::exception
        {
            public:
                virtual const char* what() const throw();
        };

    private:
        std::string							root_;
        std::string							cgi_path_;
        std::string                         return_;
        std::set<std::string>				allowed_methods_;
        std::vector<std::string>			index_;
        bool								autoindex_;
};

#endif /* LIBS_CONFIG_CONFIGLOCATION_HPP_ */
