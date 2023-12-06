#ifndef LIBS_CONFIG_BLOCK_HPP_
# define LIBS_CONFIG_BLOCK_HPP_

# include <string>       /* std::string */
# include <algorithm>    /* std::find */
# include <sys/socket.h> /* struct linger */
# include <iostream>
# include "simple.hpp"

typedef struct limit_except_s
{
    std::vector<std::string> methods;
    std::vector<std::string> allows;
    std::vector<std::string> denys;
    limit_except_s();
} limit_except_t;

typedef struct http_s
{
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
    std::vector<struct server_s> servers;
    std::map<int, std::string> error_page;
    std::map<std::string, std::string> fastcgi_param;
    http_s();
} http_t;

typedef struct server_s
{
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
    std::vector<listen_t> listens;
    std::vector<std::string> index;
    std::vector<std::string> server_name;
    std::vector<struct location_s> locations;
    std::map<int, std::string> error_page;
    std::map<std::string, std::string> fastcgi_param;
    try_files_t try_files;
    return_t ret;
    explicit server_s(const http_t &http);
} server_t;

typedef struct location_s
{
    std::string name;
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
    int keepalive_requests;
    int keepalive_time;
    keepalive_timeout_t keepalive_timeout;
    std::vector<std::string> index;
    std::vector<struct location_s> locations;
    std::map<int, std::string> error_page;
    struct limit_except_s limit_except;
    std::map<std::string, std::string> fastcgi_param;
    fastcgi_pass_t fastcgi_pass;
    try_files_t try_files;
    return_t ret;
    explicit location_s(const server_t &server);
    void print();
} location_t;

typedef struct main_s
{
    struct http_s http;
    main_s();
} main_t;

class Block
{
    public:
        static main_t ParseMain(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg);
        static http_t ParseHttp(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg);
        static server_t ParseServer(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg, \
            const http_t &http);
        static location_t ParseLocation(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg, \
            const server_t &server);
        static limit_except_t ParseLimitExcept(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg);

    private:
        ~Block();
        Block();
        Block(const Block &obj);
        Block &operator=(const Block &obj);
};

#endif /* LIBS_CONFIG_BLOCK_HPP_ */
