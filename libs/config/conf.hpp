#ifndef LIBS_CONFIG_CONF_HPP_
# define LIBS_CONFIG_CONF_HPP_

#include "config.hpp"

class Conf
{
    public:
        ~Conf();
        explicit Conf(const server_t &server, const listen_t &listen);
        Conf(const Conf &other);
        Conf &operator=(const Conf &other);

        std::string GetUrl(const std::string &str) const;
        std::string GetPath(const std::string &url) const;
        std::string GetExt(const std::string &url) const;

        int GetLocationIdx(const std::string &url) const;
        location_t GetLocation(const int idx) const;

        struct linger linger;
        std::vector<std::string> server_name;
        std::vector<struct location_s> locations;
        std::map<int, std::string> error_page;
        listen_t listen;
    private:
        Conf();

};

#endif /* !LIBS_CONFIG_CONF_HPP_ */
