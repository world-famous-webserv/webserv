#ifndef LIBS_CONFIG_BLOCK_HPP_
# define LIBS_CONFIG_BLOCK_HPP_

# include <string>  /* std::string */
# include <cstring> /* std::strcmp */
# include "simple.hpp"
# include "utils.hpp"

typedef struct BlockLimitExcept_s
{
    std::string allow_;
    std::string deny_;
} BlockLimitExcept_t;

typedef struct BlockLocation_s
{
    std::string name_;
    bool sendfile_;
    bool autoindex_;
    bool absolute_redirect_;
    bool server_name_in_redirect_;
    bool tcp_nodelay_;
    bool tcp_nopush_;
    std::string default_type_;
    std::string root_;
    std::string allow_;
    std::string deny_;
    std::string lingering_close_;
    size_t send_lowat_;
    size_t sendfile_max_chunk_;
    size_t lingering_time_;
    size_t lingering_timeout;
    size_t client_max_body_size_;
    size_t client_body_timeout_;
    size_t keepalive_requests_;
    size_t keepalive_time_;
    keepalive_timeout_t keepalive_timeout_;
    std::vector<std::string> index_;
    std::vector<struct BlockLocation_s> locations_;
    std::map<int, std::string> error_page_;
    std::map<std::string, struct BlockLimitExcept_s> limit_excepts_;
    std::map<std::string, std::string> fastcgi_param_;
    fastcgi_pass_t fastcgi_pass_;
    try_files_t try_files_;
} BlockLocation_t;

typedef struct BlockServer_s
{
    bool sendfile_;
    bool autoindex_;
    bool absolute_redirect_;
    bool server_name_in_redirect_;
    bool tcp_nodelay_;
    bool tcp_nopush_;
    std::string default_type_;
    std::string root_;
    std::string allow_;
    std::string deny_;
    std::string lingering_close_;
    size_t lingering_time_;
    size_t lingering_timeout;
    size_t send_lowat_;
    size_t sendfile_max_chunk_;
    size_t client_max_body_size_;
    size_t client_body_timeout_;
    size_t client_header_timeout_;
    size_t client_header_buffer_size_;
    size_t keepalive_requests_;
    size_t keepalive_time_;
    keepalive_timeout_t keepalive_timeout_;
    listen_t listen_;
    std::vector<std::string> index_;
    std::vector<std::string> server_name_;
    std::vector<struct BlockLocation_s> locations_;
    std::map<int, std::string> error_page_;
    std::map<std::string, std::string> fastcgi_param_;
    try_files_t try_files_;
} BlockServer_t;

typedef struct BlockHttp_s
{
    bool sendfile_;
    bool autoindex_;
    bool absolute_redirect_;
    bool server_name_in_redirect_;
    bool tcp_nodelay_;
    bool tcp_nopush_;
    std::string default_type_;
    std::string root_;
    std::string allow_;
    std::string deny_;
    std::string lingering_close_;
    size_t lingering_time_;
    size_t lingering_timeout;
    size_t send_lowat_;
    size_t sendfile_max_chunk_;
    size_t client_max_body_size_;
    size_t client_body_timeout_;
    size_t client_header_timeout_;
    size_t client_header_buffer_size_;
    size_t keepalive_requests_;
    size_t keepalive_time_;
    keepalive_timeout_t keepalive_timeout_;
    std::vector<std::string> index_;
    std::vector<struct BlockServer_s> servers_;
    std::map<int, std::string> error_page_;
    std::map<std::string, std::string> fastcgi_param_;
} BlockHttp_t;

typedef struct BlockMain_s
{
    struct BlockHttp_s http_;
} BlockMain_t;

class Block
{
    public:
        static void InitMain(BlockMain_t &main);
        static void InitHttp(BlockHttp_t &http);
        static void InitServer(BlockServer_t &server);
        static void InitLocation(BlockLocation_t &location);
        static BlockMain_t ParseMain(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg_);
        static BlockHttp_t ParseHttp(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg_);
        static BlockServer_t ParseServer(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg_);
        static BlockLocation_t ParseLocation(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg_);
        static void ParseLimitExcept(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg_, \
            std::map<std::string, struct BlockLimitExcept_s> limit_excepts_);

    private:
        ~Block();
        Block();
        Block(const Block &obj);
        Block &operator=(const Block &obj);

};

#endif /* LIBS_CONFIG_BLOCK_HPP_ */
