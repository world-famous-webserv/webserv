#ifndef LIBS_CONFIG_SIMPLE_HPP_
# define LIBS_CONFIG_SIMPLE_HPP_

# include <string>  /* std::string */
# include <map>
# include "utils.hpp"

typedef struct keepalive_timeout_s {
    size_t timeout;
    size_t header_timeout;
} keepalive_timeout_t;

typedef struct listen_s {
    std::string address;
    int port;
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
    size_t rcvbuf;
    size_t sndbuf;
    std::string accept_filter;
    std::string unixpath;
} listen_t;

typedef struct try_files_s {
    std::vector<std::string> files;
    std::string uri;
    int code;
} try_files_t;

typedef struct fastcgi_pass_s {
    std::string address;
    int port;
    bool unix;
} fastcgi_pass_t;

class Simple
{
    public:
        static bool ParseBoolType(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg_, const std::string &directive);
        static std::string ParseStringType(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg_, const std::string &directive);
        static std::vector<std::string> ParseStringVectorType(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg_, const std::string &directive);
        static size_t ParseSizeType(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg_, const std::string &directive, const bool is_time);

        static fastcgi_pass_t ParseFastcgiPass(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg_);
        static void ParseFastcgiParam(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg_, std::map<std::string, std::string> &fastcgi_param);
        static std::map<int, std::string> ParseErrorPage(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg_);
        static keepalive_timeout_t ParseKeepaliveTimeout(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg_);
        static listen_t ParseListen(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg_);
        static try_files_t ParseTryFiles(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg_);

        static void InitKeepaliveTimeout(keepalive_timeout_t &keepalive_timeout);
        static void InitListen(listen_t &listen);
        static void InitTryFiles(try_files_t &try_files);
        static void InitFastcgiPass(fastcgi_pass_t &fastcgi_pass);

    private:
        Simple();
        ~Simple();
        Simple(const Simple &obj);
        Simple &operator=(const Simple &obj);
};

# endif /* LIBS_CONFIG_SIMPLE_HPP_ */
