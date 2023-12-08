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

limit_except_s::limit_except_s(): allows(), denys() { }

location_s::location_s(const server_t &server):
    name(""),
    sendfile(server.sendfile),
    autoindex(server.autoindex),
    absolute_redirect(server.absolute_redirect),
    server_name_in_redirect(server.server_name_in_redirect),
    tcp_nodelay(server.tcp_nodelay),
    tcp_nopush(server.tcp_nopush),
    default_type(server.default_type),
    root(server.root),
    allows(server.allows),
    denys(server.denys),
    linger(server.linger),
    send_lowat(server.send_lowat),
    sendfile_max_chunk(server.sendfile_max_chunk),
    client_max_body_size(server.client_max_body_size),
    client_body_timeout(server.client_body_timeout),
    keepalive_requests(server.keepalive_requests),
    keepalive_time(server.keepalive_time),
    keepalive_timeout(server.keepalive_timeout),
    index(server.index),
    locations(std::vector<location_t>()),
    error_page(server.error_page),
    limit_excepts(std::map<std::string, limit_except_t>()),
    fastcgi_param(server.fastcgi_param),
    fastcgi_pass(),
    try_files(server.try_files),
    ret(server.ret)
{
}

server_s::server_s(const http_t &http):
    sendfile(http.sendfile),
    autoindex(http.autoindex),
    absolute_redirect(http.absolute_redirect),
    server_name_in_redirect(http.server_name_in_redirect),
    tcp_nodelay(http.tcp_nodelay),
    tcp_nopush(http.tcp_nopush),
    default_type(http.default_type),
    root(http.root),
    allows(http.allows),
    denys(http.denys),
    linger(http.linger),
    send_lowat(http.send_lowat),
    sendfile_max_chunk(http.sendfile_max_chunk),
    client_max_body_size(http.client_max_body_size),
    client_body_timeout(http.client_body_timeout),
    client_header_timeout(http.client_header_timeout),
    client_header_buffer_size(http.client_header_buffer_size),
    keepalive_requests(http.keepalive_requests),
    keepalive_time(http.keepalive_time),
    keepalive_timeout(http.keepalive_timeout),
    listens(std::vector<listen_t>()),
    index(http.index),
    server_name(std::vector<std::string>()),
    locations(std::vector<location_t>()),
    error_page(http.error_page),
    fastcgi_param(http.fastcgi_param),
    try_files(),
    ret()
{
}

http_s::http_s():
    sendfile(Simple::Default.sendfile),
    autoindex(Simple::Default.autoindex),
    absolute_redirect(Simple::Default.absolute_redirect),
    server_name_in_redirect(Simple::Default.server_name_in_redirect),
    tcp_nodelay(Simple::Default.tcp_nodelay),
    tcp_nopush(Simple::Default.tcp_nopush),
    default_type(Simple::Default.default_type),
    root(Simple::Default.root),
    allows(std::vector<std::string>()),
    denys(std::vector<std::string>()),
    linger(Simple::Default.linger),
    send_lowat(Simple::Default.send_lowat),
    sendfile_max_chunk(Simple::Default.sendfile_max_chunk),
    client_max_body_size(Simple::Default.client_max_body_size),
    client_body_timeout(Simple::Default.client_body_timeout),
    client_header_timeout(Simple::Default.client_header_timeout),
    client_header_buffer_size(Simple::Default.client_header_buffer_size),
    keepalive_requests(Simple::Default.keepalive_requests),
    keepalive_time(Simple::Default.keepalive_time),
    keepalive_timeout(Simple::Default.keepalive_timeout),
    index(Simple::Default.index),
    servers(std::vector<server_t>()),
    error_page(std::map<int, std::string>()),
    fastcgi_param(std::map<std::string, std::string>())
{
}

main_s::main_s(): http() { }

main_t Block::ParseMain(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg)
{
    main_t main;
    while (idx < tokens.size() && error_msg.empty() == true) {
        const std::string directive = tokens[idx++];
        if (directive == "http")
            main.http = ParseHttp(tokens, idx, error_msg);
        else
            error_msg = "Main: Unknown directive [ " + directive + " ]";
    }
    return main;
}

http_t Block::ParseHttp(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg)
{
    http_t http;
    if (idx == tokens.size() || tokens[idx] != "{") {
        error_msg = "Http: Missing {";
        return http;
    }

    ++idx;
    while (idx < tokens.size() && tokens[idx] != "}" && error_msg.empty() == true) {
        const std::string directive = tokens[idx++];
        if (directive == "sendfile")
            http.sendfile = Simple::ParseBool(tokens, idx, error_msg, directive);
        else if (directive == "autoindex")
            http.autoindex = Simple::ParseBool(tokens, idx, error_msg, directive);
        else if (directive == "absolute_redirect")
            http.absolute_redirect = Simple::ParseBool(tokens, idx, error_msg, directive);
        else if (directive == "server_name_in_redirect")
            http.server_name_in_redirect = Simple::ParseBool(tokens, idx, error_msg, directive);
        else if (directive == "tcp_nodelay")
            http.tcp_nodelay = Simple::ParseBool(tokens, idx, error_msg, directive);
        else if (directive == "tcp_nopush")
            http.tcp_nopush = Simple::ParseBool(tokens, idx, error_msg, directive);
        else if (directive == "default_type")
            http.default_type = Simple::ParseString(tokens, idx, error_msg, directive);
        else if (directive == "root")
            http.root = Simple::ParseString(tokens, idx, error_msg, directive);
        else if (directive == "allow")
            http.allows.push_back(Simple::ParseString(tokens, idx, error_msg, directive));
        else if (directive == "deny")
            http.denys.push_back(Simple::ParseString(tokens, idx, error_msg, directive));
        else if (directive == "lingering_close")
            http.linger.l_onoff = Simple::ParseBool(tokens, idx, error_msg, directive);
        else if (directive == "lingering_timeout")
            http.linger.l_linger = Simple::ParseInt(tokens, idx, error_msg, directive, true);
        else if (directive == "send_lowat")
            http.send_lowat = Simple::ParseInt(tokens, idx, error_msg, directive, false);
        else if (directive == "sendfile_max_chunk")
            http.sendfile_max_chunk = Simple::ParseInt(tokens, idx, error_msg, directive, false);
        else if (directive == "client_max_body_size")
            http.client_max_body_size = Simple::ParseInt(tokens, idx, error_msg, directive, false);
        else if (directive == "client_body_timeout")
            http.client_body_timeout = Simple::ParseInt(tokens, idx, error_msg, directive, true);
        else if (directive == "client_header_timeout")
            http.client_header_timeout = Simple::ParseInt(tokens, idx, error_msg, directive, true);
        else if (directive == "client_header_buffer_size")
            http.client_header_buffer_size = Simple::ParseInt(tokens, idx, error_msg, directive, false);
        else if (directive == "keepalive_requests")
            http.keepalive_requests = Simple::ParseInt(tokens, idx, error_msg, directive, true);
        else if (directive == "keepalive_time")
            http.keepalive_time = Simple::ParseInt(tokens, idx, error_msg, directive, true);
        else if (directive == "keepalive_timeout")
            http.keepalive_timeout = Simple::ParseKeepaliveTimeout(tokens, idx, error_msg);
        else if (directive == "index")
            http.index = Simple::ParseStringVector(tokens, idx, error_msg, directive);
        else if (directive == "server")
            http.servers.push_back(ParseServer(tokens, idx, error_msg, http));
        else if (directive == "error_page")
            http.error_page = Simple::ParseErrorPage(tokens, idx, error_msg);
        else if (directive == "fastcgi_param")
            Simple::ParseFastcgiParam(tokens, idx, error_msg, http.fastcgi_param);
        else
            error_msg = "Http: Unknown directive [ " + directive + " ]";
    }
    if (http.index.empty() == true)
        http.index = Simple::Default.index;
    if (idx == tokens.size())
        error_msg = "Http: Missing }";
    else
        ++idx;
    return http;
}

server_t Block::ParseServer(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg, \
    const http_t &http)
{
    server_t server(http);
    if (idx == tokens.size() || tokens[idx] != "{") {
        error_msg = "Server: Missing {";
        return server;
    }

    static bool allows_filled = true;
    static bool denys_filled = true;
    static bool factcgi_param_filled = true;

    ++idx;
    while (idx < tokens.size() && tokens[idx] != "}" && error_msg.empty() == true) {
        const std::string directive = tokens[idx++];
        if (directive == "sendfile")
            server.sendfile = Simple::ParseBool(tokens, idx, error_msg, directive);
        else if (directive == "autoindex")
            server.autoindex = Simple::ParseBool(tokens, idx, error_msg, directive);
        else if (directive == "absolute_redirect")
            server.absolute_redirect = Simple::ParseBool(tokens, idx, error_msg, directive);
        else if (directive == "server_name_in_redirect")
            server.server_name_in_redirect = Simple::ParseBool(tokens, idx, error_msg, directive);
        else if (directive == "tcp_nodelay")
            server.tcp_nodelay = Simple::ParseBool(tokens, idx, error_msg, directive);
        else if (directive == "tcp_nopush")
            server.tcp_nopush = Simple::ParseBool(tokens, idx, error_msg, directive);
        else if (directive == "default_type")
            server.default_type = Simple::ParseString(tokens, idx, error_msg, directive);
        else if (directive == "root")
            server.root = Simple::ParseString(tokens, idx, error_msg, directive);
        else if (directive == "allow") {
            if (allows_filled == true) {
                server.allows = std::vector<std::string>();
                allows_filled = false;
            }
            server.allows.push_back(Simple::ParseString(tokens, idx, error_msg, directive));
        }
        else if (directive == "deny") {
            if (denys_filled == true) {
                server.denys = std::vector<std::string>();
                denys_filled = false;
            }
            server.denys.push_back(Simple::ParseString(tokens, idx, error_msg, directive));
        }
        else if (directive == "lingering_close")
            server.linger.l_onoff = Simple::ParseBool(tokens, idx, error_msg, directive);
        else if (directive == "lingering_timeout")
            server.linger.l_linger = Simple::ParseInt(tokens, idx, error_msg, directive, true);
        else if (directive == "send_lowat")
            server.send_lowat = Simple::ParseInt(tokens, idx, error_msg, directive, false);
        else if (directive == "sendfile_max_chunk")
            server.sendfile_max_chunk = Simple::ParseInt(tokens, idx, error_msg, directive, false);
        else if (directive == "client_max_body_size")
            server.client_max_body_size = Simple::ParseInt(tokens, idx, error_msg, directive, false);
        else if (directive == "client_body_timeout")
            server.client_body_timeout = Simple::ParseInt(tokens, idx, error_msg, directive, true);
        else if (directive == "client_header_timeout")
            server.client_header_timeout = Simple::ParseInt(tokens, idx, error_msg, directive, true);
        else if (directive == "client_header_buffer_size")
            server.client_header_buffer_size = Simple::ParseInt(tokens, idx, error_msg, directive, false);
        else if (directive == "keepalive_requests")
            server.keepalive_requests = Simple::ParseInt(tokens, idx, error_msg, directive, true);
        else if (directive == "keepalive_time")
            server.keepalive_time = Simple::ParseInt(tokens, idx, error_msg, directive, true);
        else if (directive == "keepalive_timeout")
            server.keepalive_timeout = Simple::ParseKeepaliveTimeout(tokens, idx, error_msg);
        else if (directive == "listen")
            server.listens.push_back(Simple::ParseListen(tokens, idx, error_msg));
        else if (directive == "index")
            server.index = Simple::ParseStringVector(tokens, idx, error_msg, directive);
        else if (directive == "server_name")
            server.server_name = Simple::ParseStringVector(tokens, idx, error_msg, directive);
        else if (directive == "location")
            server.locations.push_back(ParseLocation(tokens, idx, error_msg, server));
        else if (directive == "error_page")
            server.error_page = Simple::ParseErrorPage(tokens, idx, error_msg);
        else if (directive == "fastcgi_param") {
            if (factcgi_param_filled == true) {
                server.fastcgi_param = std::map<std::string, std::string>();
                factcgi_param_filled = false;
            }
            Simple::ParseFastcgiParam(tokens, idx, error_msg, server.fastcgi_param);
        }
        else if (directive == "try_files")
            server.try_files = Simple::ParseTryFiles(tokens, idx, error_msg);
        else if (directive == "return")
            server.ret = Simple::ParseReturn(tokens, idx, error_msg);
        else
            error_msg = "Server: Unknown directive [ " + directive + " ]";
    }
    if (server.listens.empty() == true)
        server.listens.push_back(Simple::Default.listen);
    if (idx == tokens.size())
        error_msg = "Server: Missing }";
    else
        ++idx;
    return server;
}

location_t Block::ParseLocation(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg, const server_t &server)
{
    location_t location(server);

    if (idx == tokens.size()) {
        error_msg = "Location: Missing location name";
        return location;
    }
    location.name = tokens[idx++];

    if (idx == tokens.size() || tokens[idx] != "{") {
        error_msg = "Location: Missing {";
        return location;
    }

    static bool allows_filled = true;
    static bool denys_filled = true;
    static bool locations_filled = true;
    static bool factcgi_param_filled = true;

    ++idx;
    while (idx < tokens.size() && tokens[idx] != "}" && error_msg.empty() == true) {
        const std::string directive = tokens[idx++];
        if (directive == "sendfile")
            location.sendfile = Simple::ParseBool(tokens, idx, error_msg, directive);
        else if (directive == "autoindex")
            location.autoindex = Simple::ParseBool(tokens, idx, error_msg, directive);
        else if (directive == "absolute_redirect")
            location.absolute_redirect = Simple::ParseBool(tokens, idx, error_msg, directive);
        else if (directive == "server_name_in_redirect")
            location.server_name_in_redirect = Simple::ParseBool(tokens, idx, error_msg, directive);
        else if (directive == "tcp_nodelay")
            location.tcp_nodelay = Simple::ParseBool(tokens, idx, error_msg, directive);
        else if (directive == "tcp_nopush")
            location.tcp_nopush = Simple::ParseBool(tokens, idx, error_msg, directive);
        else if (directive == "default_type")
            location.default_type = Simple::ParseString(tokens, idx, error_msg, directive);
        else if (directive == "root")
            location.root = Simple::ParseString(tokens, idx, error_msg, directive);
        else if (directive == "allow") {
            if (allows_filled == true) {
                location.allows = std::vector<std::string>();
                allows_filled = false;
            }
            location.allows.push_back(Simple::ParseString(tokens, idx, error_msg, directive));
        }
        else if (directive == "deny") {
            if (denys_filled == true) {
                location.denys = std::vector<std::string>();
                denys_filled = false;
            }
            location.denys.push_back(Simple::ParseString(tokens, idx, error_msg, directive));
        }
        else if (directive == "lingering_close")
            location.linger.l_onoff = Simple::ParseBool(tokens, idx, error_msg, directive);
        else if (directive == "lingering_timeout")
            location.linger.l_linger = Simple::ParseInt(tokens, idx, error_msg, directive, true);
        else if (directive == "send_lowat")
            location.send_lowat = Simple::ParseInt(tokens, idx, error_msg, directive, false);
        else if (directive == "sendfile_max_chunk")
            location.sendfile_max_chunk = Simple::ParseInt(tokens, idx, error_msg, directive, false);
        else if (directive == "client_max_body_size")
            location.client_max_body_size = Simple::ParseInt(tokens, idx, error_msg, directive, false);
        else if (directive == "client_body_timeout")
            location.client_body_timeout = Simple::ParseInt(tokens, idx, error_msg, directive, true);
        else if (directive == "keepalive_requests")
            location.keepalive_requests = Simple::ParseInt(tokens, idx, error_msg, directive, true);
        else if (directive == "keepalive_time")
            location.keepalive_time = Simple::ParseInt(tokens, idx, error_msg, directive, true);
        else if (directive == "keepalive_timeout")
            location.keepalive_timeout = Simple::ParseKeepaliveTimeout(tokens, idx, error_msg);
        else if (directive == "index")
            location.index = Simple::ParseStringVector(tokens, idx, error_msg, directive);
        else if (directive == "location") {
            if (locations_filled == true) {
                location.locations = std::vector<location_t>();
                locations_filled = false;
            }
            location.locations.push_back(ParseLocation(tokens, idx, error_msg, server));
        }
        else if (directive == "error_page")
            location.error_page = Simple::ParseErrorPage(tokens, idx, error_msg);
        else if (directive == "limit_except")
            ParseLimitExcept(tokens, idx, error_msg, location.limit_excepts);
        else if (directive == "fastcgi_param") {
            if (factcgi_param_filled == true) {
                location.fastcgi_param = std::map<std::string, std::string>();
                factcgi_param_filled = false;
            }
            Simple::ParseFastcgiParam(tokens, idx, error_msg, location.fastcgi_param);
        }
        else if (directive == "fastcgi_pass")
            location.fastcgi_pass = Simple::ParseFastcgiPass(tokens, idx, error_msg);
        else if (directive == "try_files")
            location.try_files = Simple::ParseTryFiles(tokens, idx, error_msg);
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
    std::map<std::string, struct limit_except_s> limit_excepts)
{
    if (idx == tokens.size()) {
        error_msg = "LimitExcept: Missing method";
        return;
    }
    // GET, HEAD, POST, PUT, DELETE, MKCOL, COPY, MOVE, OPTIONS, PROPFIND, PROPPATCH, LOCK, UNLOCK, PATCH
    std::vector<std::string> methods;
    while (idx < tokens.size() && tokens[idx] != "{" && error_msg.empty() == true) {
        const std::string &method = tokens[idx];
        if (method == "GET" || method == "HEAD" || method == "POST" || method == "PUT" || \
            method == "DELETE" || method == "MKCOL" || method == "COPY" || method == "MOVE" || \
            method == "OPTIONS" || method == "PROPFIND" || method == "PROPPATCH" || method == "LOCK" || \
            method == "UNLOCK" || method == "PATCH")
        {
            if (std::find(methods.begin(), methods.end(), method) == methods.end())
                methods.push_back(method);
            ++idx;
        } else {
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

    std::vector<std::string> allows;
    std::vector<std::string> denys;
    while (idx < tokens.size() && tokens[idx] != "}" && error_msg.empty() == true) {
        const std::string directive = tokens[idx++];
        if (directive == "allow")
            allows.push_back(Simple::ParseString(tokens, idx, error_msg, directive));
        else if (directive == "deny")
            denys.push_back(Simple::ParseString(tokens, idx, error_msg, directive));
        else {
            error_msg = "LimitExcept: Unknown directive [ " + directive + " ]";
            return;
        }
    }

    if (idx == tokens.size()) {
        error_msg = "LimitExcept: Missing }";
        return;
    } else if (error_msg.empty() == false)
        return;
    else
        ++idx;

    for (std::vector<std::string>::const_iterator cur = methods.begin(); cur != methods.end(); ++cur) {
        if (limit_excepts.find(*cur) == limit_excepts.end())
            limit_excepts[*cur] = limit_except_s();
        limit_excepts[*cur].allows = allows;
        limit_excepts[*cur].denys = denys;
    }
}

void location_s::print() {
    std::cout << "location" << '\n';
    std::cout << "name: " << name << '\n';
    std::cout << "sendfile: " << sendfile << '\n';
    std::cout << "autoindex: " << autoindex << '\n';
    std::cout << "absolute_redirect: " << absolute_redirect << '\n';
    std::cout << "server_name_in_redirect: " << server_name_in_redirect << '\n';
    std::cout << "tcp_nodelay: " << tcp_nodelay << '\n';
    std::cout << "tcp_nopush: " << tcp_nopush << '\n';
    std::cout << "default_type: " << default_type << '\n';
    std::cout << "root: " << root << '\n';
    for (size_t i = 0; i < allows.size(); ++i)
        std::cout << "allow: " << allows[i] << '\n';
    for (size_t i = 0; i < denys.size(); ++i)
        std::cout << "deny: " << denys[i] << '\n';
    std::cout << "lingering_close: " << linger.l_onoff << '\n';
    std::cout << "lingering_timeout: " << linger.l_linger << '\n';
    std::cout << "send_lowat: " << send_lowat << '\n';
    std::cout << "sendfile_max_chunk: " << sendfile_max_chunk << '\n';
    std::cout << "client_max_body_size: " << client_max_body_size << '\n';
    std::cout << "client_body_timeout: " << client_body_timeout << '\n';
    std::cout << "keepalive_requests: " << keepalive_requests << '\n';
    std::cout << "keepalive_time: " << keepalive_time << '\n';
    std::cout << "keepalive_timeout.timeout: " << keepalive_timeout.timeout << '\n';
    std::cout << "keepalive_timeout.header_timeout: " << keepalive_timeout.header_timeout << '\n';
    std::cout << "index:";
    for (size_t i = 0; i < index.size(); i++)
        std::cout << " " << index[i];
    std::cout << '\n' << '\n';
    std::cout << "error_page:" << '\n';
    for (std::map<int, std::string>::const_iterator it = error_page.begin(); it != error_page.end(); ++it)
        std::cout << it->first << " = " << it->second << '\n';
    std::cout << '\n';
    std::cout << "fastcgi_param:" << '\n';
    for (std::map<std::string, std::string>::const_iterator it = fastcgi_param.begin(); it != fastcgi_param.end(); ++it)
        std::cout << it->first << " = " << it->second << '\n';
    std::cout << "try_files:" << '\n';
    std::cout << "    uri: " << try_files.uri << '\n';
    std::cout << "    code: " << try_files.code << '\n';
    std::cout << "    files:";
    for (size_t i = 0; i < try_files.files.size(); i++)
        std::cout << " " << try_files.files[i];
    std::cout << '\n';
    std::cout << "fastcgi_pass:" << '\n';
    std::cout << "    address: " << fastcgi_pass.address << '\n';
    std::cout << "    port: " << fastcgi_pass.port << '\n';
    std::cout << "    unix: " << fastcgi_pass.unix << '\n';
    std::cout << "\n";
}
