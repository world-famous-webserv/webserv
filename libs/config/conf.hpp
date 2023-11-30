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

        int GetLocationIdx(const std::string &url) const;
        location_t GetLocation(const int idx) const;

        bool sendfile;
        bool autoindex;
        bool absolute_redirect;
        bool server_name_in_redirect;
        bool tcp_nodelay;
        bool tcp_nopush;
        std::string default_type;
        std::string root;
        std::vector<std::string> allows;
        std::vector<std::string> denys;
        struct linger linger;
        int send_lowat;
        int sendfile_max_chunk;
        int client_max_body_size;
        int client_body_timeout;
        int client_header_timeout;
        int client_header_buffer_size;
        int keepalive_requests;
        int keepalive_time;
        keepalive_timeout_t keepalive_timeout;
        std::vector<std::string> index;
        std::vector<std::string> server_name;
        std::vector<struct location_s> locations;
        std::map<int, std::string> error_page;
        std::map<std::string, std::string> fastcgi_param;
        try_files_t try_files;
        return_t ret;
        listen_t listen;
    private:
        Conf();

};

#endif /* !LIBS_CONFIG_CONF_HPP_ */
