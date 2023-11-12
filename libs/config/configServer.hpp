#ifndef LIBS_CONFIG_CONFIGSERVER_HPP_
#define LIBS_CONFIG_CONFIGSERVER_HPP_

#include <string>
#include <vector>
#include <cstdlib>
#include <map>
#include "configStringUtils.hpp"
#include "configLocation.hpp"

class ConfigServer
{
    public:
        ~ConfigServer();
        ConfigServer();
        ConfigServer(const ConfigServer &obj);
        ConfigServer& operator=(const ConfigServer &obj);
		static ConfigServer parseServer(const std::vector<std::string> &tokens, std::vector<std::string>::size_type &i);

        class BadFileFormatException: public std::exception
        {
            public:
                virtual const char* what() const throw();
        };

    private:
		int										listen_;
		std::string								host_;
		std::string								server_name_;
		std::string								root_;
		int										client_max_body_size_;
		std::map<int, std::string>				error_page_;
		std::vector<std::string>				index_;
		std::map<std::string, ConfigLocation>	location_;
};

#endif /* LIBS_CONFIG_CONFIGSERVER_HPP_ */
