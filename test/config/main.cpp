#include <iostream>
#include <vector>
#include "config.hpp"

void print_return(const return_t &ret)
{
    std::cout << "return" << '\n';
    std::cout << "    code: " << ret.code << '\n';
    std::cout << "    url: " << ret.url << '\n';
    std::cout << "    text: " << ret.text << '\n';
    std::cout << '\n';
}

void print_http(const BlockHttp_t &http)
{
    std::cout << "http" << '\n';
    std::cout << "sendfile: " << http.sendfile << '\n';
    std::cout << "autoindex: " << http.autoindex << '\n';
    std::cout << "absolute_redirect: " << http.absolute_redirect << '\n';
    std::cout << "server_name_in_redirect: " << http.server_name_in_redirect << '\n';
    std::cout << "tcp_nodelay: " << http.tcp_nodelay << '\n';
    std::cout << "tcp_nopush: " << http.tcp_nopush << '\n';
    std::cout << "default_type: " << http.default_type << '\n';
    std::cout << "root: " << http.root << '\n';
    for (size_t i = 0; i < http.allows.size(); ++i)
        std::cout << "allow: " << http.allows[i] << '\n';
    for (size_t i = 0; i < http.denys.size(); ++i)
        std::cout << "deny: " << http.denys[i] << '\n';
    std::cout << "lingering_close: " << http.linger.l_onoff << '\n';
    std::cout << "lingering_timeout: " << http.linger.l_linger << '\n';
    std::cout << "send_lowat: " << http.send_lowat << '\n';
    std::cout << "sendfile_max_chunk: " << http.sendfile_max_chunk << '\n';
    std::cout << "client_max_body_size: " << http.client_max_body_size << '\n';
    std::cout << "client_body_timeout: " << http.client_body_timeout << '\n';
    std::cout << "client_header_timeout: " << http.client_header_timeout << '\n';
    std::cout << "client_header_buffer_size: " << http.client_header_buffer_size << '\n';
    std::cout << "keepalive_requests: " << http.keepalive_requests << '\n';
    std::cout << "keepalive_time: " << http.keepalive_time << '\n';
    std::cout << "keepalive_timeout.timeout: " << http.keepalive_timeout.timeout << '\n';
    std::cout << "keepalive_timeout.header_timeout: " << http.keepalive_timeout.header_timeout << '\n';
    std::cout << "index:";
    for (size_t i = 0; i < http.index.size(); i++)
        std::cout << " " << http.index[i];
    std::cout << '\n' << '\n';
    std::cout << "error_page:" << '\n';
    for (std::map<int, std::string>::const_iterator it = http.error_page.begin(); it != http.error_page.end(); ++it)
        std::cout << it->first << " = " << it->second << '\n';
    std::cout << '\n';
    std::cout << "fastcgi_param:" << '\n';
    for (std::map<std::string, std::string>::const_iterator it = http.fastcgi_param.begin(); it != http.fastcgi_param.end(); ++it)
        std::cout << it->first << " = " << it->second << '\n';
    std::cout << '\n';
}

void print_listen(const listen_t &listen) {
    std::cout << "listen" << '\n';
    std::cout << "address: " << listen.address << '\n';
    std::cout << "port: " << listen.port << '\n';
    std::cout << "default_server: " << listen.default_server << '\n';
    std::cout << "ssl: " << listen.ssl << '\n';
    std::cout << "http2: " << listen.http2 << '\n';
    std::cout << "quic: " << listen.quic << '\n';
    std::cout << "proxy_protocol: " << listen.proxy_protocol << '\n';
    std::cout << "deferred: " << listen.deferred << '\n';
    std::cout << "bind: " << listen.bind << '\n';
    std::cout << "ipv6only: " << listen.ipv6only << '\n';
    std::cout << "reuseport: " << listen.reuseport << '\n';
    std::cout << "so_keepalive: " << listen.so_keepalive << '\n';
    std::cout << "keepidle: " << listen.keepidle << '\n';
    std::cout << "keepintvl: " << listen.keepintvl << '\n';
    std::cout << "keepcnt: " << listen.keepcnt << '\n';
    std::cout << "setfib: " << listen.setfib << '\n';
    std::cout << "fastopen: " << listen.fastopen << '\n';
    std::cout << "backlog: " << listen.backlog << '\n';
    std::cout << "rcvbuf: " << listen.rcvbuf << '\n';
    std::cout << "sndbuf: " << listen.sndbuf << '\n';
    std::cout << "accept_filter: " << listen.accept_filter << '\n';
    std::cout << "unixpath: " << listen.unixpath << '\n';
}

void print_server(const BlockServer_t &server) {
    std::cout << "server" << '\n';
    std::cout << "sendfile: " << server.sendfile << '\n';
    std::cout << "autoindex: " << server.autoindex << '\n';
    std::cout << "absolute_redirect: " << server.absolute_redirect << '\n';
    std::cout << "server_name_in_redirect: " << server.server_name_in_redirect << '\n';
    std::cout << "tcp_nodelay: " << server.tcp_nodelay << '\n';
    std::cout << "tcp_nopush: " << server.tcp_nopush << '\n';
    std::cout << "default_type: " << server.default_type << '\n';
    std::cout << "root: " << server.root << '\n';
    for (size_t i = 0; i < server.allows.size(); ++i)
        std::cout << "allow: " << server.allows[i] << '\n';
    for (size_t i = 0; i < server.denys.size(); ++i)
        std::cout << "deny: " << server.denys[i] << '\n';
    std::cout << "lingering_close: " << server.linger.l_onoff << '\n';
    std::cout << "lingering_timeout: " << server.linger.l_linger << '\n';
    std::cout << "send_lowat: " << server.send_lowat << '\n';
    std::cout << "sendfile_max_chunk: " << server.sendfile_max_chunk << '\n';
    std::cout << "client_max_body_size: " << server.client_max_body_size << '\n';
    std::cout << "client_body_timeout: " << server.client_body_timeout << '\n';
    std::cout << "client_header_timeout: " << server.client_header_timeout << '\n';
    std::cout << "client_header_buffer_size: " << server.client_header_buffer_size << '\n';
    std::cout << "keepalive_requests: " << server.keepalive_requests << '\n';
    std::cout << "keepalive_time: " << server.keepalive_time << '\n';
    std::cout << "keepalive_timeout.timeout: " << server.keepalive_timeout.timeout << '\n';
    std::cout << "keepalive_timeout.header_timeout: " << server.keepalive_timeout.header_timeout << '\n';
    print_listen(server.listens[0]);
    std::cout << "index:";
    for (size_t i = 0; i < server.index.size(); i++)
        std::cout << " " << server.index[i];
    std::cout << "\n\n";
    std::cout << "server_name:";
    for (size_t i = 0; i < server.server_name.size(); i++)
        std::cout << " " << server.server_name[i];
    std::cout << "\n\n";
    std::cout << "error_page:" << '\n';
    for (std::map<int, std::string>::const_iterator it = server.error_page.begin(); it != server.error_page.end(); ++it)
        std::cout << it->first << " = " << it->second << '\n';
    std::cout << '\n';
    std::cout << "fastcgi_param:" << '\n';
    for (std::map<std::string, std::string>::const_iterator it = server.fastcgi_param.begin(); it != server.fastcgi_param.end(); ++it)
        std::cout << it->first << " = " << it->second << '\n';
    std::cout << "    try_files:" << '\n';
    std::cout << "    uri: " << server.try_files.uri << '\n';
    std::cout << "    code: " << server.try_files.code << '\n';
    std::cout << "    files:";
    for (size_t i = 0; i < server.try_files.files.size(); ++i)
        std::cout << " " << server.try_files.files[i];
    std::cout << '\n';
    print_return(server.ret);
}

void print_location(const BlockLocation_t &location) {
    std::cout << "location" << '\n';
    std::cout << "name: " << location.name << '\n';
    std::cout << "sendfile: " << location.sendfile << '\n';
    std::cout << "autoindex: " << location.autoindex << '\n';
    std::cout << "absolute_redirect: " << location.absolute_redirect << '\n';
    std::cout << "server_name_in_redirect: " << location.server_name_in_redirect << '\n';
    std::cout << "tcp_nodelay: " << location.tcp_nodelay << '\n';
    std::cout << "tcp_nopush: " << location.tcp_nopush << '\n';
    std::cout << "default_type: " << location.default_type << '\n';
    std::cout << "root: " << location.root << '\n';
    for (size_t i = 0; i < location.allows.size(); ++i)
        std::cout << "allow: " << location.allows[i] << '\n';
    for (size_t i = 0; i < location.denys.size(); ++i)
        std::cout << "deny: " << location.denys[i] << '\n';
    std::cout << "lingering_close: " << location.linger.l_onoff << '\n';
    std::cout << "lingering_timeout: " << location.linger.l_linger << '\n';
    std::cout << "send_lowat: " << location.send_lowat << '\n';
    std::cout << "sendfile_max_chunk: " << location.sendfile_max_chunk << '\n';
    std::cout << "client_max_body_size: " << location.client_max_body_size << '\n';
    std::cout << "client_body_timeout: " << location.client_body_timeout << '\n';
    std::cout << "keepalive_requests: " << location.keepalive_requests << '\n';
    std::cout << "keepalive_time: " << location.keepalive_time << '\n';
    std::cout << "keepalive_timeout.timeout: " << location.keepalive_timeout.timeout << '\n';
    std::cout << "keepalive_timeout.header_timeout: " << location.keepalive_timeout.header_timeout << '\n';
    std::cout << "index:";
    for (size_t i = 0; i < location.index.size(); i++)
        std::cout << " " << location.index[i];
    std::cout << '\n' << '\n';
    std::cout << "error_page:" << '\n';
    for (std::map<int, std::string>::const_iterator it = location.error_page.begin(); it != location.error_page.end(); ++it)
        std::cout << it->first << " = " << it->second << '\n';
    std::cout << '\n';
    std::cout << "fastcgi_param:" << '\n';
    for (std::map<std::string, std::string>::const_iterator it = location.fastcgi_param.begin(); it != location.fastcgi_param.end(); ++it)
        std::cout << it->first << " = " << it->second << '\n';
    std::cout << "try_files:" << '\n';
    std::cout << "    uri: " << location.try_files.uri << '\n';
    std::cout << "    code: " << location.try_files.code << '\n';
    std::cout << "    files:";
    for (size_t i = 0; i < location.try_files.files.size(); i++)
        std::cout << " " << location.try_files.files[i];
    std::cout << '\n';
    std::cout << "fastcgi_pass:" << '\n';
    std::cout << "    address: " << location.fastcgi_pass.address << '\n';
    std::cout << "    port: " << location.fastcgi_pass.port << '\n';
    std::cout << "    unix: " << location.fastcgi_pass.unix << '\n';
    std::cout << "\n";
    print_return(location.ret);
}

int main(int argc, const char *argv[])
{
    if (argc > 2) {
        std::cerr << "Usage: " << argv[0] << " [configuration file] | None" << '\n';
        return EXIT_FAILURE;
    }

    std::string file;
    if (argc == 1)
        file = "../../data/default.conf";
    else
        file = argv[1];

    Config config(file);

    if (config.IsOpen() == false) {
        std::cerr << "Error: " << config.ErrorMsg() << '\n';
        return EXIT_SUCCESS;
    }
    
    const BlockHttp_t http = config.GetHttp();
    print_http(http);

    const BlockServer_t server = config.GetServer("localhost:8080");
    print_server(server);

    const std::string url = "/kapouet/pouic/toto/pouet";
    const std::string path = config.GetPath(url);
    std::cout << "url: " << url << " path: " << path << '\n';

    const BlockLocation_t location = config.GetLocation(url);
    print_location(location);

    return EXIT_SUCCESS;
}
