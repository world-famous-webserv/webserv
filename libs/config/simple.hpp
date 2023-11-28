#ifndef LIBS_CONFIG_SIMPLE_HPP_
# define LIBS_CONFIG_SIMPLE_HPP_

# include <string>       /* std::string */
# include <map>          /* std::map */
# include <sys/socket.h> /* struct linger */
# include "utils.hpp"

typedef struct listen_s {
    std::string address;
    std::string port;
    bool default_server;
    bool ssl;
    bool http2;
    bool quic;
    bool proxy_protocol;
    bool deferred;
    bool bind;
    bool ipv6only;
    bool reuseport;
    bool so_keepalive;
    int keepidle;
    int keepintvl;
    int keepcnt;
    int setfib;
    int fastopen;
    int backlog;
    int rcvbuf;
    int sndbuf;
    std::string accept_filter;
    std::string unixpath;
    listen_s();
} listen_t;

typedef struct return_s {
    int code;
    std::string url;
    std::string text;
    return_s();
} return_t;

typedef struct keepalive_timeout_s {
    int timeout;
    int header_timeout;
    keepalive_timeout_s();
} keepalive_timeout_t;

typedef struct try_files_s {
    std::vector<std::string> files;
    std::string uri;
    int code;
    try_files_s();
} try_files_t;

typedef struct fastcgi_pass_s {
    std::string address;
    std::string port;
    int unix;
    fastcgi_pass_s();
} fastcgi_pass_t;

typedef struct default_s {
    listen_t listen;
    return_t ret;
    keepalive_timeout_t keepalive_timeout;
    try_files_t try_files;
    fastcgi_pass_t fastcgi_pass;
    int sendfile;
    int autoindex;
    int absolute_redirect;
    int server_name_in_redirect;
    int tcp_nodelay;
    int tcp_nopush;
    std::string default_type;
    std::string root;
    std::string index;
    std::string server_name;
    struct linger linger;
    int send_lowat;
    int sendfile_max_chunk;
    int client_max_body_size;
    int client_body_timeout;
    int client_header_timeout;
    int client_header_buffer_size;
    int keepalive_requests;
    int keepalive_time;
    default_s();
} default_t;

class Simple
{
    public:
        static int ParseBoolType(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg, const std::string &directive);
        static std::string ParseStringType(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg, const std::string &directive);
        static std::vector<std::string> ParseStringVectorType(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg, const std::string &directive);
        static int ParseIntType(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg, const std::string &directive, const bool is_time);

        static fastcgi_pass_t ParseFastcgiPass(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg);
        static void ParseFastcgiParam(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg, std::map<std::string, std::string> &fastcgi_param);
        static std::map<int, std::string> ParseErrorPage(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg);
        static keepalive_timeout_t ParseKeepaliveTimeout(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg);
        static listen_t ParseListen(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg);
        static try_files_t ParseTryFiles(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg);
        static return_t ParseReturn(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg);

        static default_t Default;

    private:
        Simple();
        ~Simple();
        Simple(const Simple &obj);
        Simple &operator=(const Simple &obj);
};

# endif /* LIBS_CONFIG_SIMPLE_HPP_ */
