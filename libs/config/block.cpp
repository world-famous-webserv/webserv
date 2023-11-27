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

BlockLimitExcept_s::BlockLimitExcept_s(): allows(), denys() { }

BlockLocation_s::BlockLocation_s():
    name(""),
    sendfile(false),
    autoindex(false),
    absolute_redirect(true),
    server_name_in_redirect(false),
    tcp_nodelay(true),
    tcp_nopush(false),
    default_type("text/html"),
    root("html"),
    allows(),
    denys(),
    lingering_close("on"),
    send_lowat(0),
    sendfile_max_chunk(2 * 1024 * 1024), \
    lingering_time(30),
    lingering_timeout(5),
    client_max_body_size(1 * 1024 * 1024),
    client_body_timeout(60),
    keepalive_requests(1000),
    keepalive_time(1 * 60 * 60)
{
    index.push_back("index.html");
}

BlockServer_s::BlockServer_s(): \
    sendfile(false),
    autoindex(false),
    absolute_redirect(true),
    server_name_in_redirect(false),
    tcp_nodelay(true),
    tcp_nopush(false),
    default_type("text/html"),
    root("html"),
    allows(),
    denys(),
    lingering_close("on"),
    lingering_time(30),
    lingering_timeout(5),
    send_lowat(0),
    sendfile_max_chunk(2 * 1024 * 1024),
    client_max_body_size(1 * 1024 * 1024),
    client_body_timeout(60),
    client_header_timeout(60),
    client_header_buffer_size(1 * 1024),
    keepalive_requests(1000),
    keepalive_time(1 * 60 * 60)
{
    server_name.push_back("");
    index.push_back("index.html");
}

BlockHttp_s::BlockHttp_s():
    sendfile(false),
    autoindex(false),
    absolute_redirect(true),
    server_name_in_redirect(false),
    tcp_nodelay(true),
    tcp_nopush(false),
    default_type("text/html"),
    root("html"),
    allows(),
    denys(),
    lingering_close("on"),
    lingering_time(30),
    lingering_timeout(5),
    send_lowat(0),
    sendfile_max_chunk(2 * 1024 * 1024),
    client_max_body_size(1 * 1024 * 1024),
    client_body_timeout(60),
    client_header_timeout(60),
    client_header_buffer_size(1 * 1024),
    keepalive_requests(1000),
    keepalive_time(1 * 60 * 60)
{
    index.push_back("index.html");
}

BlockMain_s::BlockMain_s(): http() { }

BlockMain_t Block::ParseMain(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg)
{
    BlockMain_t main;
    while (idx < tokens.size() && error_msg.empty() == true) {
        const std::string directive = tokens[idx++];
        if (directive == "http")
            main.http = ParseHttp(tokens, idx, error_msg);
        else
            error_msg = "Main: Unknown directive [ " + directive + " ]";
    }
    return main;
}

BlockHttp_t Block::ParseHttp(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg)
{
    BlockHttp_t http;
    if (idx == tokens.size() || tokens[idx] != "{") {
        error_msg = "Http: Missing {";
        return http;
    }
    ++idx;
    while (idx < tokens.size() && tokens[idx] != "}" && error_msg.empty() == true) {
        const std::string directive = tokens[idx++];
        if (directive == "server")
            http.servers.push_back(ParseServer(tokens, idx, error_msg));
        else if (directive == "default_type")
            http.default_type = Simple::ParseStringType(tokens, idx, error_msg, directive);
        else if (directive == "sendfile")
            http.sendfile = Simple::ParseBoolType(tokens, idx, error_msg, directive);
        else if (directive == "keepalive_timeout")
            http.keepalive_timeout = Simple::ParseKeepaliveTimeout(tokens, idx, error_msg);
        else if (directive == "root")
            http.root = Simple::ParseStringType(tokens, idx, error_msg, directive);
        else if (directive == "client_max_body_size")
            http.client_max_body_size = Simple::ParseSizeType(tokens, idx, error_msg, directive, false);
        else if (directive == "client_body_timeout")
            http.client_body_timeout = Simple::ParseSizeType(tokens, idx, error_msg, directive, true);
        else if (directive == "client_header_timeout")
            http.client_header_timeout = Simple::ParseSizeType(tokens, idx, error_msg, directive, true);
        else if (directive == "client_header_buffer_size")
            http.client_header_buffer_size = Simple::ParseSizeType(tokens, idx, error_msg, directive, false);
        else if (directive == "keepalive_requests")
            http.keepalive_requests = Simple::ParseSizeType(tokens, idx, error_msg, directive, true);
        else if (directive == "keepalive_time")
            http.keepalive_time = Simple::ParseSizeType(tokens, idx, error_msg, directive, true);
        else if (directive == "index")
            http.index = Simple::ParseStringVectorType(tokens, idx, error_msg, directive);
        else if (directive == "error_page")
            http.error_page = Simple::ParseErrorPage(tokens, idx, error_msg);
        else if (directive == "autoindex")
            http.autoindex = Simple::ParseBoolType(tokens, idx, error_msg, directive);
        else if (directive == "allow")
            http.allows.push_back(Simple::ParseStringType(tokens, idx, error_msg, directive));
        else if (directive == "deny")
            http.denys.push_back(Simple::ParseStringType(tokens, idx, error_msg, directive));
        else if (directive == "absolute_redirect")
            http.absolute_redirect = Simple::ParseBoolType(tokens, idx, error_msg, directive);
        else if (directive == "lingering_close") {
            http.lingering_close = Simple::ParseStringType(tokens, idx, error_msg, directive);
            if (http.lingering_close != "on" && http.lingering_close != "off" && http.lingering_close != "always") {
                error_msg = "Http: Invalid value for [ " + directive + " ]";
                return http;
            }
        } else if (directive == "lingering_time")
            http.lingering_time = Simple::ParseSizeType(tokens, idx, error_msg, directive, true);
        else if (directive == "lingering_timeout")
            http.lingering_timeout = Simple::ParseSizeType(tokens, idx, error_msg, directive, true);
        else if (directive == "send_lowat")
            http.send_lowat = Simple::ParseSizeType(tokens, idx, error_msg, directive, false);
        else if (directive == "sendfile_max_chunk")
            http.sendfile_max_chunk = Simple::ParseSizeType(tokens, idx, error_msg, directive, false);
        else if (directive == "server_name_in_redirect")
            http.server_name_in_redirect = Simple::ParseBoolType(tokens, idx, error_msg, directive);
        else if (directive == "tcp_nodelay")
            http.tcp_nodelay = Simple::ParseBoolType(tokens, idx, error_msg, directive);
        else if (directive == "tcp_nopush")
            http.tcp_nopush = Simple::ParseBoolType(tokens, idx, error_msg, directive);
        else if (directive == "fastcgi_param")
            Simple::ParseFastcgiParam(tokens, idx, error_msg, http.fastcgi_param);
        else
            error_msg = "Http: Unknown directive [ " + directive + " ]";
    }
    if (idx == tokens.size())
        error_msg = "Http: Missing }";
    else
        ++idx;
    return http;
}

BlockServer_t Block::ParseServer(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg)
{
    BlockServer_t server;
    if (idx == tokens.size() || tokens[idx] != "{") {
        error_msg = "Server: Missing {";
        return server;
    }
    ++idx;
    while (idx < tokens.size() && tokens[idx] != "}" && error_msg.empty() == true) {
        const std::string directive = tokens[idx++];
        if (directive == "location")
            server.locations.push_back(ParseLocation(tokens, idx, error_msg));
        else if (directive == "default_type")
            server.default_type = Simple::ParseStringType(tokens, idx, error_msg, directive);
        else if (directive == "sendfile")
            server.sendfile = Simple::ParseBoolType(tokens, idx, error_msg, directive);
        else if (directive == "keepalive_timeout")
            server.keepalive_timeout = Simple::ParseKeepaliveTimeout(tokens, idx, error_msg);
        else if (directive == "listen")
            server.listens.push_back(Simple::ParseListen(tokens, idx, error_msg));
        else if (directive == "server_name")
            server.server_name = Simple::ParseStringVectorType(tokens, idx, error_msg, directive);
        else if (directive == "root")
            server.root = Simple::ParseStringType(tokens, idx, error_msg, directive);
        else if (directive == "client_max_body_size")
            server.client_max_body_size = Simple::ParseSizeType(tokens, idx, error_msg, directive, false);
        else if (directive == "client_body_timeout")
            server.client_body_timeout = Simple::ParseSizeType(tokens, idx, error_msg, directive, true);
        else if (directive == "client_header_timeout")
            server.client_header_timeout = Simple::ParseSizeType(tokens, idx, error_msg, directive, true);
        else if (directive == "client_header_buffer_size")
            server.client_header_buffer_size = Simple::ParseSizeType(tokens, idx, error_msg, directive, false);
        else if (directive == "keepalive_requests")
            server.keepalive_requests = Simple::ParseSizeType(tokens, idx, error_msg, directive, true);
        else if (directive == "keepalive_time")
            server.keepalive_time = Simple::ParseSizeType(tokens, idx, error_msg, directive, true);
        else if (directive == "index")
            server.index = Simple::ParseStringVectorType(tokens, idx, error_msg, directive);
        else if (directive == "error_page")
            server.error_page = Simple::ParseErrorPage(tokens, idx, error_msg);
        else if (directive == "autoindex")
            server.autoindex = Simple::ParseBoolType(tokens, idx, error_msg, directive);
        else if (directive == "allow")
            server.allows.push_back(Simple::ParseStringType(tokens, idx, error_msg, directive));
        else if (directive == "deny")
            server.denys.push_back(Simple::ParseStringType(tokens, idx, error_msg, directive));
        else if (directive == "absolute_redirect")
            server.absolute_redirect = Simple::ParseBoolType(tokens, idx, error_msg, directive);
        else if (directive == "lingering_close") {
            server.lingering_close = Simple::ParseStringType(tokens, idx, error_msg, directive);
            if (server.lingering_close != "on" && server.lingering_close != "off" && server.lingering_close != "always") {
                error_msg = "Server: Invalid value for [ " + directive + " ]";
                return server;
            }
        } else if (directive == "lingering_time")
            server.lingering_time = Simple::ParseSizeType(tokens, idx, error_msg, directive, true);
        else if (directive == "lingering_timeout")
            server.lingering_timeout = Simple::ParseSizeType(tokens, idx, error_msg, directive, true);
        else if (directive == "send_lowat")
            server.send_lowat = Simple::ParseSizeType(tokens, idx, error_msg, directive, false);
        else if (directive == "sendfile_max_chunk")
            server.sendfile_max_chunk = Simple::ParseSizeType(tokens, idx, error_msg, directive, false);
        else if (directive == "server_name_in_redirect")
            server.server_name_in_redirect = Simple::ParseBoolType(tokens, idx, error_msg, directive);
        else if (directive == "tcp_nodelay")
            server.tcp_nodelay = Simple::ParseBoolType(tokens, idx, error_msg, directive);
        else if (directive == "tcp_nopush")
            server.tcp_nopush = Simple::ParseBoolType(tokens, idx, error_msg, directive);
        else if (directive == "try_files")
            server.try_files = Simple::ParseTryFiles(tokens, idx, error_msg);
        else if (directive == "fastcgi_param")
            Simple::ParseFastcgiParam(tokens, idx, error_msg, server.fastcgi_param);
        else if (directive == "return")
            server.ret = Simple::ParseReturn(tokens, idx, error_msg);
        else
            error_msg = "Server: Unknown directive [ " + directive + " ]";
    }

    if (idx == tokens.size())
        error_msg = "Server: Missing }";
    else
        ++idx;
    return server;
}

BlockLocation_t Block::ParseLocation(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg)
{
    BlockLocation_t location;

    if (idx == tokens.size()) {
        error_msg = "Location: Missing location name";
        return location;
    }
    location.name = tokens[idx++];
    if (idx == tokens.size() || tokens[idx] != "{") {
        error_msg = "Location: Missing {";
        return location;
    }
    ++idx;
    while (idx < tokens.size() && tokens[idx] != "}" && error_msg.empty() == true) {
        const std::string directive = tokens[idx++];
        if (directive == "location")
            location.locations.push_back(ParseLocation(tokens, idx, error_msg));
        else if (directive == "default_type")
            location.default_type = Simple::ParseStringType(tokens, idx, error_msg, directive);
        else if (directive == "sendfile")
            location.sendfile = Simple::ParseBoolType(tokens, idx, error_msg, directive);
        else if (directive == "keepalive_timeout")
            location.keepalive_timeout = Simple::ParseKeepaliveTimeout(tokens, idx, error_msg);
        else if (directive == "root")
            location.root = Simple::ParseStringType(tokens, idx, error_msg, directive);
        else if (directive == "client_max_body_size")
            location.client_max_body_size = Simple::ParseSizeType(tokens, idx, error_msg, directive, false);
        else if (directive == "client_body_timeout")
            location.client_body_timeout = Simple::ParseSizeType(tokens, idx, error_msg, directive, true);
        else if (directive == "keepalive_requests")
            location.keepalive_requests = Simple::ParseSizeType(tokens, idx, error_msg, directive, true);
        else if (directive == "keepalive_time")
            location.keepalive_time = Simple::ParseSizeType(tokens, idx, error_msg, directive, true);
        else if (directive == "index")
            location.index = Simple::ParseStringVectorType(tokens, idx, error_msg, directive);
        else if (directive == "error_page")
            location.error_page = Simple::ParseErrorPage(tokens, idx, error_msg);
        else if (directive == "autoindex")
            location.autoindex = Simple::ParseBoolType(tokens, idx, error_msg, directive);
        else if (directive == "limit_except")
            ParseLimitExcept(tokens, idx, error_msg, location.limit_excepts);
        else if (directive == "allow")
            location.allows.push_back(Simple::ParseStringType(tokens, idx, error_msg, directive));
        else if (directive == "deny")
            location.denys.push_back(Simple::ParseStringType(tokens, idx, error_msg, directive));
        else if (directive == "absolute_redirect")
            location.absolute_redirect = Simple::ParseBoolType(tokens, idx, error_msg, directive);
        else if (directive == "lingering_close") {
            location.lingering_close = Simple::ParseStringType(tokens, idx, error_msg, directive);
            if (location.lingering_close != "on" && location.lingering_close != "off" && location.lingering_close != "always") {
                error_msg = "Location: Invalid value for [ " + directive + " ]";
                return location;
            }
        } else if (directive == "lingering_time")
            location.lingering_time = Simple::ParseSizeType(tokens, idx, error_msg, directive, true);
        else if (directive == "lingering_timeout")
            location.lingering_timeout = Simple::ParseSizeType(tokens, idx, error_msg, directive, true);
        else if (directive == "send_lowat")
            location.send_lowat = Simple::ParseSizeType(tokens, idx, error_msg, directive, false);
        else if (directive == "sendfile_max_chunk")
            location.sendfile_max_chunk = Simple::ParseSizeType(tokens, idx, error_msg, directive, false);
        else if (directive == "server_name_in_redirect")
            location.server_name_in_redirect = Simple::ParseBoolType(tokens, idx, error_msg, directive);
        else if (directive == "tcp_nodelay")
            location.tcp_nodelay = Simple::ParseBoolType(tokens, idx, error_msg, directive);
        else if (directive == "tcp_nopush")
            location.tcp_nopush = Simple::ParseBoolType(tokens, idx, error_msg, directive);
        else if (directive == "try_files")
            location.try_files = Simple::ParseTryFiles(tokens, idx, error_msg);
        else if (directive == "fastcgi_param")
            Simple::ParseFastcgiParam(tokens, idx, error_msg, location.fastcgi_param);
        else if (directive == "fastcgi_pass")
            location.fastcgi_pass = Simple::ParseFastcgiPass(tokens, idx, error_msg);
        else if (directive == "return")
            location.ret = Simple::ParseReturn(tokens, idx, error_msg);
        else
            error_msg = "Location: Unknown directive [ " + directive + " ] at location [ " + location.name + " ]";
    }
    if (idx == tokens.size())
        error_msg = "Location: Missing }";
    else
        ++idx;
    return location;
}

void Block::ParseLimitExcept(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg, \
    std::map<std::string, struct BlockLimitExcept_s> limit_excepts)
{
    if (idx == tokens.size()) {
        error_msg = "LimitExcept: Missing method";
        return;
    }
    // GET, HEAD, POST, PUT, DELETE, MKCOL, COPY, MOVE, OPTIONS, PROPFIND, PROPPATCH, LOCK, UNLOCK, PATCH
    std::vector<std::string> methods;
    while (idx < tokens.size() && tokens[idx] != "{" && error_msg.empty() == true) {
        const std::string method = tokens[idx];
        if (method == "GET" || method == "HEAD" || method == "POST" || method == "PUT" || method == "DELETE" || \
            method == "MKCOL" || method == "COPY" || method == "MOVE" || method == "OPTIONS" || method == "PROPFIND" || method == "PROPPATCH" || \
            method == "LOCK" || method == "UNLOCK" || method == "PATCH") {
                if (std::find(methods.begin(), methods.end(), method) != methods.end()) {
                    error_msg = "LimitExcept: Duplicate method [ " + method + " ]";
                    return;
                }
                else {
                    methods.push_back(method);
                    ++idx;
                }
            }
        else {
            error_msg = "LimitExcept: Unknown method [ " + method + " ]";
            return;
        }
    }
    if (methods.empty() == true) {
        error_msg = "LimitExcept: Missing method";
        return;
    }
    if (idx == tokens.size() || tokens[idx] != "{") {
        error_msg = "LimitExcept: Missing {";
        return;
    }
    ++idx;
    std::string allow("");
    std::string deny("");

    while (idx < tokens.size() && tokens[idx] != "}" && error_msg.empty() == true) {
        const std::string directive = tokens[idx++];
        if (directive == "allow") {
            if (allow.empty() == false)
                error_msg = "LimitExcept: Duplicate allow";
            else
                allow = Simple::ParseStringType(tokens, idx, error_msg, directive);
        }
        else if (directive == "deny") {
            if (deny.empty() == false)
                error_msg = "LimitExcept: Duplicate deny";
            else
                deny = Simple::ParseStringType(tokens, idx, error_msg, directive);
        }
        else
            error_msg = "LimitExcept: Unknown directive [ " + directive + " ]";
    }

    for (std::vector<std::string>::size_type i = 0, end = methods.size(); i < end; ++i) {
        if (limit_excepts.find(methods[i]) == limit_excepts.end())
            limit_excepts[methods[i]] = BlockLimitExcept_s();
        limit_excepts[methods[i]].allows.push_back(allow);
        limit_excepts[methods[i]].denys.push_back(deny);
    }
    if (idx == tokens.size())
        error_msg = "LimitExcept: Missing }";
    else
        ++idx;
}
