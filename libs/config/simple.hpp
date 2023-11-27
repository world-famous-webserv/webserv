#ifndef LIBS_CONFIG_SIMPLE_HPP_
# define LIBS_CONFIG_SIMPLE_HPP_

# include <string>    /* std::string */
# include <map>       /* std::map */
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
    size_t keepidle;
    size_t keepintvl;
    size_t keepcnt;
    size_t setfib;
    size_t fastopen;
    size_t backlog;
    size_t rcvbuf;
    size_t sndbuf;
    std::string accept_filter;
    std::string unixpath;
    listen_s();
} listen_t;

typedef struct return_s {
    size_t code;
    std::string url;
    std::string text;
    return_s();
} return_t;

typedef struct keepalive_timeout_s {
    size_t timeout;
    size_t header_timeout;
    keepalive_timeout_s();
} keepalive_timeout_t;

typedef struct try_files_s {
    std::vector<std::string> files;
    std::string uri;
    size_t code;
    try_files_s();
} try_files_t;

typedef struct fastcgi_pass_s {
    std::string address;
    size_t port;
    bool unix;
    fastcgi_pass_s();
} fastcgi_pass_t;

class Simple
{
    public:
        static bool ParseBoolType(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg, const std::string &directive);
        static std::string ParseStringType(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg, const std::string &directive);
        static std::vector<std::string> ParseStringVectorType(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg, const std::string &directive);
        static size_t ParseSizeType(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg, const std::string &directive, const bool is_time);

        static fastcgi_pass_t ParseFastcgiPass(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg);
        static void ParseFastcgiParam(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg, std::map<std::string, std::string> &fastcgi_param);
        static std::map<size_t, std::string> ParseErrorPage(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg);
        static keepalive_timeout_t ParseKeepaliveTimeout(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg);
        static listen_t ParseListen(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg);
        static try_files_t ParseTryFiles(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg);
        static return_t ParseReturn(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg);

    private:
        Simple();
        ~Simple();
        Simple(const Simple &obj);
        Simple &operator=(const Simple &obj);
};

# endif /* LIBS_CONFIG_SIMPLE_HPP_ */
