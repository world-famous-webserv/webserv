#ifndef LIBS_CONFIG_BLOCK_HPP_
# define LIBS_CONFIG_BLOCK_HPP_

# include <string>       /* std::string */
# include <algorithm>    /* std::find */
# include <sys/socket.h> /* struct linger */
# include "simple.hpp"
# include "utils.hpp"
# include <iostream>

struct BlockMain_s;
struct BlockHttp_s;
struct BlockServer_s;
struct BlockLocation_s;
struct BlockLimitExcept_s;

typedef struct BlockHttp_s
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
    std::vector<struct BlockServer_s> servers;
    std::map<int, std::string> error_page;
    std::map<std::string, std::string> fastcgi_param;
    BlockHttp_s();
} BlockHttp_t;

typedef struct BlockServer_s
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
    std::vector<struct BlockLocation_s> locations;
    std::map<int, std::string> error_page;
    std::map<std::string, std::string> fastcgi_param;
    try_files_t try_files;
    return_t ret;
    explicit BlockServer_s(const BlockHttp_t &http);
} BlockServer_t;

typedef struct BlockLocation_s
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
    std::vector<struct BlockLocation_s> locations;
    std::map<int, std::string> error_page;
    std::map<std::string, struct BlockLimitExcept_s> limit_excepts;
    std::map<std::string, std::string> fastcgi_param;
    fastcgi_pass_t fastcgi_pass;
    try_files_t try_files;
    return_t ret;
    explicit BlockLocation_s(const BlockServer_t &server);
} BlockLocation_t;

typedef struct BlockLimitExcept_s
{
    std::vector<std::string> allows;
    std::vector<std::string> denys;
    BlockLimitExcept_s();
} BlockLimitExcept_t;

typedef struct BlockMain_s
{
    struct BlockHttp_s http;
    BlockMain_s();
} BlockMain_t;

class Block
{
    public:
        static BlockMain_t ParseMain(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg);
        static BlockHttp_t ParseHttp(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg);
        static BlockServer_t ParseServer(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg, \
            const BlockHttp_t &http);
        static BlockLocation_t ParseLocation(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg, \
            const BlockServer_t &server);
        static void ParseLimitExcept(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg, \
            std::map<std::string, struct BlockLimitExcept_s> limit_excepts);

    private:
        ~Block();
        Block();
        Block(const Block &obj);
        Block &operator=(const Block &obj);
};

#endif /* LIBS_CONFIG_BLOCK_HPP_ */
