#include "block.hpp"

Block::~Block() { }
Block::Block() { }
Block::Block(const Block &obj) { *this = obj; }
Block &Block::operator=(const Block &obj)
{
    if (this == &obj)
        return *this;
    return *this;
}

void Block::InitMain(BlockMain_t &main)
{
    InitHttp(main.http_);
}

void Block::InitHttp(BlockHttp_t &http)
{
    http.sendfile_ = false;
    http.autoindex_ = false;
    http.absolute_redirect_ = true;
    http.server_name_in_redirect_ = false;
    http.tcp_nodelay_ = true;
    http.tcp_nopush_ = false;
    http.default_type_ = "text/html";
    http.root_ = "html";
    http.lingering_close_ = "on";
    http.lingering_time_ = 30;
    http.lingering_timeout = 5;
    http.send_lowat_ = 0;
    http.sendfile_max_chunk_ = 2 * 1024 * 1024;
    http.client_max_body_size_ = 1 * 1024 * 1024;
    http.client_body_timeout_ = 60;
    http.client_header_timeout_ = 60;
    http.client_header_buffer_size_ = 1 * 1024;
    http.keepalive_requests_ = 1000;
    http.keepalive_time_ = 1 * 60 * 60;
    http.index_.push_back("index.html");
    Simple::InitKeepaliveTimeout(http.keepalive_timeout_);
}

void Block::InitServer(BlockServer_t &server)
{
    server.sendfile_ = false;
    server.autoindex_ = false;
    server.absolute_redirect_ = true;
    server.server_name_in_redirect_ = false;
    server.tcp_nodelay_ = true;
    server.tcp_nopush_ = false;
    server.default_type_ = "text/html";
    server.root_ = "html";
    server.lingering_close_ = "on";
    server.lingering_time_ = 30;
    server.lingering_timeout = 5;
    server.send_lowat_ = 0;
    server.sendfile_max_chunk_ = 2 * 1024 * 1024;
    server.client_max_body_size_ = 1 * 1024 * 1024;
    server.client_body_timeout_ = 60;
    server.client_header_timeout_ = 60;
    server.client_header_buffer_size_ = 1 * 1024;
    server.keepalive_requests_ = 1000;
    server.keepalive_time_ = 1 * 60 * 60;
    server.server_name_.push_back("");
    server.index_.push_back("index.html");
    Simple::InitKeepaliveTimeout(server.keepalive_timeout_);
    Simple::InitListen(server.listen_);
    Simple::InitTryFiles(server.try_files_);
    Simple::InitReturn(server.return_);
}

void Block::InitLocation(BlockLocation_t &location)
{
    location.sendfile_ = false;
    location.autoindex_ = false;
    location.absolute_redirect_ = true;
    location.server_name_in_redirect_ = false;
    location.tcp_nodelay_ = true;
    location.tcp_nopush_ = false;
    location.default_type_ = "text/html";
    location.root_ = "html";
    location.lingering_close_ = "on";
    location.lingering_time_ = 30;
    location.lingering_timeout = 5;
    location.send_lowat_ = 0;
    location.sendfile_max_chunk_ = 2 * 1024 * 1024;
    location.client_max_body_size_ = 1 * 1024 * 1024;
    location.client_body_timeout_ = 60;
    location.keepalive_requests_ = 1000;
    location.keepalive_time_ = 1 * 60 * 60;
    location.index_.push_back("index.html");
    Simple::InitKeepaliveTimeout(location.keepalive_timeout_);
    Simple::InitTryFiles(location.try_files_);
    Simple::InitFastcgiPass(location.fastcgi_pass_);
    Simple::InitReturn(location.return_);
}

BlockMain_t Block::ParseMain(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg_)
{
    BlockMain_t main; InitMain(main);
    while (idx < tokens.size() && error_msg_.empty() == true) {
        const std::string directive = tokens[idx++];
        if (directive == "http")
            main.http_ = ParseHttp(tokens, idx, error_msg_);
        else
            error_msg_ = "Main: Unknown directive [ " + directive + " ]";
    }
    return main;
}

BlockHttp_t Block::ParseHttp(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg_)
{
    BlockHttp_t http; InitHttp(http);
    if (idx == tokens.size() || tokens[idx] != "{") {
        error_msg_ = "Http: Missing {";
        return http;
    }
    ++idx;
    while (idx < tokens.size() && tokens[idx] != "}" && error_msg_.empty() == true) {
        const std::string directive = tokens[idx++];
        if (directive == "server")
            http.servers_.push_back(ParseServer(tokens, idx, error_msg_));
        else if (directive == "default_type")
            http.default_type_ = Simple::ParseStringType(tokens, idx, error_msg_, directive);
        else if (directive == "sendfile")
            http.sendfile_ = Simple::ParseBoolType(tokens, idx, error_msg_, directive);
        else if (directive == "keepalive_timeout")
            http.keepalive_timeout_ = Simple::ParseKeepaliveTimeout(tokens, idx, error_msg_);
        else if (directive == "root")
            http.root_ = Simple::ParseStringType(tokens, idx, error_msg_, directive);
        else if (directive == "client_max_body_size")
            http.client_max_body_size_ = Simple::ParseSizeType(tokens, idx, error_msg_, directive, false);
        else if (directive == "client_body_timeout")
            http.client_body_timeout_ = Simple::ParseSizeType(tokens, idx, error_msg_, directive, true);
        else if (directive == "client_header_timeout")
            http.client_header_timeout_ = Simple::ParseSizeType(tokens, idx, error_msg_, directive, true);
        else if (directive == "client_header_buffer_size")
            http.client_header_buffer_size_ = Simple::ParseSizeType(tokens, idx, error_msg_, directive, false);
        else if (directive == "keepalive_requests")
            http.keepalive_requests_ = Simple::ParseSizeType(tokens, idx, error_msg_, directive, true);
        else if (directive == "keepalive_time")
            http.keepalive_time_ = Simple::ParseSizeType(tokens, idx, error_msg_, directive, true);
        else if (directive == "index")
            http.index_ = Simple::ParseStringVectorType(tokens, idx, error_msg_, directive);
        else if (directive == "error_page")
            http.error_page_ = Simple::ParseErrorPage(tokens, idx, error_msg_);
        else if (directive == "autoindex")
            http.autoindex_ = Simple::ParseBoolType(tokens, idx, error_msg_, directive);
        else if (directive == "allow")
            http.allow_ = Simple::ParseStringType(tokens, idx, error_msg_, directive);
        else if (directive == "deny")
            http.deny_ = Simple::ParseStringType(tokens, idx, error_msg_, directive);
        else if (directive == "absolute_redirect")
            http.absolute_redirect_ = Simple::ParseBoolType(tokens, idx, error_msg_, directive);
        else if (directive == "lingering_close") {
            http.lingering_close_ = Simple::ParseStringType(tokens, idx, error_msg_, directive);
            if (http.lingering_close_ != "on" && http.lingering_close_ != "off" && http.lingering_close_ != "always") {
                error_msg_ = "Http: Invalid value for [ " + directive + " ]";
                return http;
            }
        } else if (directive == "lingering_time")
            http.lingering_time_ = Simple::ParseSizeType(tokens, idx, error_msg_, directive, true);
        else if (directive == "lingering_timeout")
            http.lingering_timeout = Simple::ParseSizeType(tokens, idx, error_msg_, directive, true);
        else if (directive == "send_lowat")
            http.send_lowat_ = Simple::ParseSizeType(tokens, idx, error_msg_, directive, false);
        else if (directive == "sendfile_max_chunk")
            http.sendfile_max_chunk_ = Simple::ParseSizeType(tokens, idx, error_msg_, directive, false);
        else if (directive == "server_name_in_redirect")
            http.server_name_in_redirect_ = Simple::ParseBoolType(tokens, idx, error_msg_, directive);
        else if (directive == "tcp_nodelay")
            http.tcp_nodelay_ = Simple::ParseBoolType(tokens, idx, error_msg_, directive);
        else if (directive == "tcp_nopush")
            http.tcp_nopush_ = Simple::ParseBoolType(tokens, idx, error_msg_, directive);
        else if (directive == "fastcgi_param")
            Simple::ParseFastcgiParam(tokens, idx, error_msg_, http.fastcgi_param_);
        else
            error_msg_ = "Http: Unknown directive [ " + directive + " ]";
    }
    if (idx == tokens.size())
        error_msg_ = "Http: Missing }";
    else
        ++idx;
    return http;
}

BlockServer_t Block::ParseServer(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg_)
{
    BlockServer_t server; InitServer(server);
    if (idx == tokens.size() || tokens[idx] != "{") {
        error_msg_ = "Server: Missing {";
        return server;
    }
    ++idx;
    while (idx < tokens.size() && tokens[idx] != "}" && error_msg_.empty() == true) {
        const std::string directive = tokens[idx++];
        if (directive == "location")
            server.locations_.push_back(ParseLocation(tokens, idx, error_msg_));
        else if (directive == "default_type")
            server.default_type_ = Simple::ParseStringType(tokens, idx, error_msg_, directive);
        else if (directive == "sendfile")
            server.sendfile_ = Simple::ParseBoolType(tokens, idx, error_msg_, directive);
        else if (directive == "keepalive_timeout")
            server.keepalive_timeout_ = Simple::ParseKeepaliveTimeout(tokens, idx, error_msg_);
        else if (directive == "listen")
            server.listen_ = Simple::ParseListen(tokens, idx, error_msg_);
        else if (directive == "server_name")
            server.server_name_ = Simple::ParseStringVectorType(tokens, idx, error_msg_, directive);
        else if (directive == "root")
            server.root_ = Simple::ParseStringType(tokens, idx, error_msg_, directive);
        else if (directive == "client_max_body_size")
            server.client_max_body_size_ = Simple::ParseSizeType(tokens, idx, error_msg_, directive, false);
        else if (directive == "client_body_timeout")
            server.client_body_timeout_ = Simple::ParseSizeType(tokens, idx, error_msg_, directive, true);
        else if (directive == "client_header_timeout")
            server.client_header_timeout_ = Simple::ParseSizeType(tokens, idx, error_msg_, directive, true);
        else if (directive == "client_header_buffer_size")
            server.client_header_buffer_size_ = Simple::ParseSizeType(tokens, idx, error_msg_, directive, false);
        else if (directive == "keepalive_requests")
            server.keepalive_requests_ = Simple::ParseSizeType(tokens, idx, error_msg_, directive, true);
        else if (directive == "keepalive_time")
            server.keepalive_time_ = Simple::ParseSizeType(tokens, idx, error_msg_, directive, true);
        else if (directive == "index")
            server.index_ = Simple::ParseStringVectorType(tokens, idx, error_msg_, directive);
        else if (directive == "error_page")
            server.error_page_ = Simple::ParseErrorPage(tokens, idx, error_msg_);
        else if (directive == "autoindex")
            server.autoindex_ = Simple::ParseBoolType(tokens, idx, error_msg_, directive);
        else if (directive == "allow")
            server.allow_ = Simple::ParseStringType(tokens, idx, error_msg_, directive);
        else if (directive == "deny")
            server.deny_ = Simple::ParseStringType(tokens, idx, error_msg_, directive);
        else if (directive == "absolute_redirect")
            server.absolute_redirect_ = Simple::ParseBoolType(tokens, idx, error_msg_, directive);
        else if (directive == "lingering_close") {
            server.lingering_close_ = Simple::ParseStringType(tokens, idx, error_msg_, directive);
            if (server.lingering_close_ != "on" && server.lingering_close_ != "off" && server.lingering_close_ != "always") {
                error_msg_ = "Server: Invalid value for [ " + directive + " ]";
                return server;
            }
        } else if (directive == "lingering_time")
            server.lingering_time_ = Simple::ParseSizeType(tokens, idx, error_msg_, directive, true);
        else if (directive == "lingering_timeout")
            server.lingering_timeout = Simple::ParseSizeType(tokens, idx, error_msg_, directive, true);
        else if (directive == "send_lowat")
            server.send_lowat_ = Simple::ParseSizeType(tokens, idx, error_msg_, directive, false);
        else if (directive == "sendfile_max_chunk")
            server.sendfile_max_chunk_ = Simple::ParseSizeType(tokens, idx, error_msg_, directive, false);
        else if (directive == "server_name_in_redirect")
            server.server_name_in_redirect_ = Simple::ParseBoolType(tokens, idx, error_msg_, directive);
        else if (directive == "tcp_nodelay")
            server.tcp_nodelay_ = Simple::ParseBoolType(tokens, idx, error_msg_, directive);
        else if (directive == "tcp_nopush")
            server.tcp_nopush_ = Simple::ParseBoolType(tokens, idx, error_msg_, directive);
        else if (directive == "try_files")
            server.try_files_ = Simple::ParseTryFiles(tokens, idx, error_msg_);
        else if (directive == "fastcgi_param")
            Simple::ParseFastcgiParam(tokens, idx, error_msg_, server.fastcgi_param_);
        else if (directive == "return")
            server.return_ = Simple::ParseReturn(tokens, idx, error_msg_);
        else
            error_msg_ = "Server: Unknown directive [ " + directive + " ]";
    }
    if (idx == tokens.size())
        error_msg_ = "Server: Missing }";
    else
        ++idx;
    return server;
}

BlockLocation_t Block::ParseLocation(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg_)
{
    BlockLocation_t location; InitLocation(location);

    if (idx == tokens.size()) {
        error_msg_ = "Location: Missing location name";
        return location;
    }
    location.name_ = tokens[idx++];
    if (idx == tokens.size() || tokens[idx] != "{") {
        error_msg_ = "Location: Missing {";
        return location;
    }
    ++idx;
    while (idx < tokens.size() && tokens[idx] != "}" && error_msg_.empty() == true) {
        const std::string directive = tokens[idx++];
        if (directive == "location")
            location.locations_.push_back(ParseLocation(tokens, idx, error_msg_));
        else if (directive == "default_type")
            location.default_type_ = Simple::ParseStringType(tokens, idx, error_msg_, directive);
        else if (directive == "sendfile")
            location.sendfile_ = Simple::ParseBoolType(tokens, idx, error_msg_, directive);
        else if (directive == "keepalive_timeout")
            location.keepalive_timeout_ = Simple::ParseKeepaliveTimeout(tokens, idx, error_msg_);
        else if (directive == "root")
            location.root_ = Simple::ParseStringType(tokens, idx, error_msg_, directive);
        else if (directive == "client_max_body_size")
            location.client_max_body_size_ = Simple::ParseSizeType(tokens, idx, error_msg_, directive, false);
        else if (directive == "client_body_timeout")
            location.client_body_timeout_ = Simple::ParseSizeType(tokens, idx, error_msg_, directive, true);
        else if (directive == "keepalive_requests")
            location.keepalive_requests_ = Simple::ParseSizeType(tokens, idx, error_msg_, directive, true);
        else if (directive == "keepalive_time")
            location.keepalive_time_ = Simple::ParseSizeType(tokens, idx, error_msg_, directive, true);
        else if (directive == "index")
            location.index_ = Simple::ParseStringVectorType(tokens, idx, error_msg_, directive);
        else if (directive == "error_page")
            location.error_page_ = Simple::ParseErrorPage(tokens, idx, error_msg_);
        else if (directive == "autoindex")
            location.autoindex_ = Simple::ParseBoolType(tokens, idx, error_msg_, directive);
        else if (directive == "limit_except")
            ParseLimitExcept(tokens, idx, error_msg_, location.limit_excepts_);
        else if (directive == "allow")
            location.allow_ = Simple::ParseStringType(tokens, idx, error_msg_, directive);
        else if (directive == "deny")
            location.deny_ = Simple::ParseStringType(tokens, idx, error_msg_, directive);
        else if (directive == "absolute_redirect")
            location.absolute_redirect_ = Simple::ParseBoolType(tokens, idx, error_msg_, directive);
        else if (directive == "lingering_close") {
            location.lingering_close_ = Simple::ParseStringType(tokens, idx, error_msg_, directive);
            if (location.lingering_close_ != "on" && location.lingering_close_ != "off" && location.lingering_close_ != "always") {
                error_msg_ = "Location: Invalid value for [ " + directive + " ]";
                return location;
            }
        } else if (directive == "lingering_time")
            location.lingering_time_ = Simple::ParseSizeType(tokens, idx, error_msg_, directive, true);
        else if (directive == "lingering_timeout")
            location.lingering_timeout = Simple::ParseSizeType(tokens, idx, error_msg_, directive, true);
        else if (directive == "send_lowat")
            location.send_lowat_ = Simple::ParseSizeType(tokens, idx, error_msg_, directive, false);
        else if (directive == "sendfile_max_chunk")
            location.sendfile_max_chunk_ = Simple::ParseSizeType(tokens, idx, error_msg_, directive, false);
        else if (directive == "server_name_in_redirect")
            location.server_name_in_redirect_ = Simple::ParseBoolType(tokens, idx, error_msg_, directive);
        else if (directive == "tcp_nodelay")
            location.tcp_nodelay_ = Simple::ParseBoolType(tokens, idx, error_msg_, directive);
        else if (directive == "tcp_nopush")
            location.tcp_nopush_ = Simple::ParseBoolType(tokens, idx, error_msg_, directive);
        else if (directive == "try_files")
            location.try_files_ = Simple::ParseTryFiles(tokens, idx, error_msg_);
        else if (directive == "fastcgi_param")
            Simple::ParseFastcgiParam(tokens, idx, error_msg_, location.fastcgi_param_);
        else if (directive == "fastcgi_pass")
            location.fastcgi_pass_ = Simple::ParseFastcgiPass(tokens, idx, error_msg_);
        else if (directive == "return")
            location.return_ = Simple::ParseReturn(tokens, idx, error_msg_);
        else
            error_msg_ = "Location: Unknown directive [ " + directive + " ] at location [ " + location.name_ + " ]";
    }
    if (idx == tokens.size())
        error_msg_ = "Location: Missing }";
    else
        ++idx;
    return location;
}

void Block::ParseLimitExcept(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg_, \
    std::map<std::string, struct BlockLimitExcept_s> limit_excepts_)
{
    if (idx == tokens.size()) {
        error_msg_ = "LimitExcept: Missing method";
        return;
    }
    // GET, HEAD, POST, PUT, DELETE, MKCOL, COPY, MOVE, OPTIONS, PROPFIND, PROPPATCH, LOCK, UNLOCK, PATCH
    std::vector<std::string> methods;
    while (idx < tokens.size() && tokens[idx] != "{" && error_msg_.empty() == true) {
        const std::string method = tokens[idx];
        if (method == "GET" || method == "HEAD" || method == "POST" || method == "PUT" || method == "DELETE" || \
            method == "MKCOL" || method == "COPY" || method == "MOVE" || method == "OPTIONS" || method == "PROPFIND" || method == "PROPPATCH" || \
            method == "LOCK" || method == "UNLOCK" || method == "PATCH") {
                if (std::find(methods.begin(), methods.end(), method) != methods.end()) {
                    error_msg_ = "LimitExcept: Duplicate method [ " + method + " ]";
                    return;
                }
                else {
                    methods.push_back(method);
                    ++idx;
                }
            }
        else {
            error_msg_ = "LimitExcept: Unknown method [ " + method + " ]";
            return;
        }
    }
    if (methods.empty() == true) {
        error_msg_ = "LimitExcept: Missing method";
        return;
    }
    if (idx == tokens.size() || tokens[idx] != "{") {
        error_msg_ = "LimitExcept: Missing {";
        return;
    }
    ++idx;
    std::string allow("");
    std::string deny("");

    while (idx < tokens.size() && tokens[idx] != "}" && error_msg_.empty() == true) {
        const std::string directive = tokens[idx++];
        if (directive == "allow") {
            if (allow.empty() == false)
                error_msg_ = "LimitExcept: Duplicate allow";
            else
                allow = Simple::ParseStringType(tokens, idx, error_msg_, directive);
        }
        else if (directive == "deny") {
            if (deny.empty() == false)
                error_msg_ = "LimitExcept: Duplicate deny";
            else
                deny = Simple::ParseStringType(tokens, idx, error_msg_, directive);
        }
        else
            error_msg_ = "LimitExcept: Unknown directive [ " + directive + " ]";
    }

    for (std::vector<std::string>::size_type i = 0, end = methods.size(); i < end; ++i) {
        BlockLimitExcept_t limit_except = {allow, deny};
        limit_excepts_[methods[i]] = limit_except;
    }
    if (idx == tokens.size())
        error_msg_ = "LimitExcept: Missing }";
    else
        ++idx;
}
