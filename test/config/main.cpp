#include <iostream>
#include <vector>
#include "config.hpp"

void print_http(const BlockHttp_t &http)
{
    std::cout << "http" << '\n';
    std::cout << "sendfile: " << http.sendfile_ << '\n';
    std::cout << "autoindex: " << http.autoindex_ << '\n';
    std::cout << "absolute_redirect: " << http.absolute_redirect_ << '\n';
    std::cout << "server_name_in_redirect: " << http.server_name_in_redirect_ << '\n';
    std::cout << "tcp_nodelay: " << http.tcp_nodelay_ << '\n';
    std::cout << "tcp_nopush: " << http.tcp_nopush_ << '\n';
    std::cout << "default_type: " << http.default_type_ << '\n';
    std::cout << "root: " << http.root_ << '\n';
    std::cout << "allow: " << http.allow_ << '\n';
    std::cout << "deny: " << http.deny_ << '\n';
    std::cout << "lingering_close: " << http.lingering_close_ << '\n';
    std::cout << "lingering_time: " << http.lingering_time_ << '\n';
    std::cout << "lingering_timeout: " << http.lingering_timeout << '\n';
    std::cout << "send_lowat: " << http.send_lowat_ << '\n';
    std::cout << "sendfile_max_chunk: " << http.sendfile_max_chunk_ << '\n';
    std::cout << "client_max_body_size: " << http.client_max_body_size_ << '\n';
    std::cout << "client_body_timeout: " << http.client_body_timeout_ << '\n';
    std::cout << "client_header_timeout: " << http.client_header_timeout_ << '\n';
    std::cout << "client_header_buffer_size: " << http.client_header_buffer_size_ << '\n';
    std::cout << "keepalive_requests: " << http.keepalive_requests_ << '\n';
    std::cout << "keepalive_time: " << http.keepalive_time_ << '\n';
    std::cout << "keepalive_timeout.timeout: " << http.keepalive_timeout_.timeout << '\n';
    std::cout << "keepalive_timeout.header_timeout: " << http.keepalive_timeout_.header_timeout << '\n';
    std::cout << "index:";
    for (size_t i = 0; i < http.index_.size(); i++)
        std::cout << " " << http.index_[i];
    std::cout << '\n' << '\n';
    std::cout << "error_page:" << '\n';
    for (std::map<int, std::string>::const_iterator it = http.error_page_.begin(); it != http.error_page_.end(); it++)
        std::cout << it->first << " = " << it->second << '\n';
    std::cout << '\n';
    std::cout << "fastcgi_param:" << '\n';
    for (std::map<std::string, std::string>::const_iterator it = http.fastcgi_param_.begin(); it != http.fastcgi_param_.end(); it++)
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
    std::cout << "sendfile: " << server.sendfile_ << '\n';
    std::cout << "autoindex: " << server.autoindex_ << '\n';
    std::cout << "absolute_redirect: " << server.absolute_redirect_ << '\n';
    std::cout << "server_name_in_redirect: " << server.server_name_in_redirect_ << '\n';
    std::cout << "tcp_nodelay: " << server.tcp_nodelay_ << '\n';
    std::cout << "tcp_nopush: " << server.tcp_nopush_ << '\n';
    std::cout << "default_type: " << server.default_type_ << '\n';
    std::cout << "root: " << server.root_ << '\n';
    std::cout << "allow: " << server.allow_ << '\n';
    std::cout << "deny: " << server.deny_ << '\n';
    std::cout << "lingering_close: " << server.lingering_close_ << '\n';
    std::cout << "lingering_time: " << server.lingering_time_ << '\n';
    std::cout << "lingering_timeout: " << server.lingering_timeout << '\n';
    std::cout << "send_lowat: " << server.send_lowat_ << '\n';
    std::cout << "sendfile_max_chunk: " << server.sendfile_max_chunk_ << '\n';
    std::cout << "client_max_body_size: " << server.client_max_body_size_ << '\n';
    std::cout << "client_body_timeout: " << server.client_body_timeout_ << '\n';
    std::cout << "client_header_timeout: " << server.client_header_timeout_ << '\n';
    std::cout << "client_header_buffer_size: " << server.client_header_buffer_size_ << '\n';
    std::cout << "keepalive_requests: " << server.keepalive_requests_ << '\n';
    std::cout << "keepalive_time: " << server.keepalive_time_ << '\n';
    std::cout << "keepalive_timeout.timeout: " << server.keepalive_timeout_.timeout << '\n';
    std::cout << "keepalive_timeout.header_timeout: " << server.keepalive_timeout_.header_timeout << '\n';
    print_listen(server.listen_);
    std::cout << "index:";
    for (size_t i = 0; i < server.index_.size(); i++)
        std::cout << " " << server.index_[i];
    std::cout << "\n\n";
    std::cout << "server_name:";
    for (size_t i = 0; i < server.server_name_.size(); i++)
        std::cout << " " << server.server_name_[i];
    std::cout << "\n\n";
    std::cout << "error_page:" << '\n';
    for (std::map<int, std::string>::const_iterator it = server.error_page_.begin(); it != server.error_page_.end(); it++)
        std::cout << it->first << " = " << it->second << '\n';
    std::cout << '\n';
    std::cout << "fastcgi_param:" << '\n';
    for (std::map<std::string, std::string>::const_iterator it = server.fastcgi_param_.begin(); it != server.fastcgi_param_.end(); it++)
        std::cout << it->first << " = " << it->second << '\n';
    std::cout << "    try_files:" << '\n';
    std::cout << "    uri: " << server.try_files_.uri << '\n';
    std::cout << "    code: " << server.try_files_.code << '\n';
    std::cout << "    files:";
    for (size_t i = 0; i < server.try_files_.files.size(); i++)
        std::cout << " " << server.try_files_.files[i];
    std::cout << '\n';
}

void print_location(const BlockLocation_t &location) {
    std::cout << "location" << '\n';
    std::cout << "name: " << location.name_ << '\n';
    std::cout << "sendfile: " << location.sendfile_ << '\n';
    std::cout << "autoindex: " << location.autoindex_ << '\n';
    std::cout << "absolute_redirect: " << location.absolute_redirect_ << '\n';
    std::cout << "server_name_in_redirect: " << location.server_name_in_redirect_ << '\n';
    std::cout << "tcp_nodelay: " << location.tcp_nodelay_ << '\n';
    std::cout << "tcp_nopush: " << location.tcp_nopush_ << '\n';
    std::cout << "default_type: " << location.default_type_ << '\n';
    std::cout << "root: " << location.root_ << '\n';
    std::cout << "allow: " << location.allow_ << '\n';
    std::cout << "deny: " << location.deny_ << '\n';
    std::cout << "lingering_close: " << location.lingering_close_ << '\n';
    std::cout << "lingering_time: " << location.lingering_time_ << '\n';
    std::cout << "lingering_timeout: " << location.lingering_timeout << '\n';
    std::cout << "send_lowat: " << location.send_lowat_ << '\n';
    std::cout << "sendfile_max_chunk: " << location.sendfile_max_chunk_ << '\n';
    std::cout << "client_max_body_size: " << location.client_max_body_size_ << '\n';
    std::cout << "client_body_timeout: " << location.client_body_timeout_ << '\n';
    std::cout << "keepalive_requests: " << location.keepalive_requests_ << '\n';
    std::cout << "keepalive_time: " << location.keepalive_time_ << '\n';
    std::cout << "keepalive_timeout.timeout: " << location.keepalive_timeout_.timeout << '\n';
    std::cout << "keepalive_timeout.header_timeout: " << location.keepalive_timeout_.header_timeout << '\n';
    std::cout << "index:";
    for (size_t i = 0; i < location.index_.size(); i++)
        std::cout << " " << location.index_[i];
    std::cout << '\n' << '\n';
    std::cout << "error_page:" << '\n';
    for (std::map<int, std::string>::const_iterator it = location.error_page_.begin(); it != location.error_page_.end(); it++)
        std::cout << it->first << " = " << it->second << '\n';
    std::cout << '\n';
    std::cout << "fastcgi_param:" << '\n';
    for (std::map<std::string, std::string>::const_iterator it = location.fastcgi_param_.begin(); it != location.fastcgi_param_.end(); it++)
        std::cout << it->first << " = " << it->second << '\n';
    std::cout << "try_files:" << '\n';
    std::cout << "    uri: " << location.try_files_.uri << '\n';
    std::cout << "    code: " << location.try_files_.code << '\n';
    std::cout << "    files:";
    for (size_t i = 0; i < location.try_files_.files.size(); i++)
        std::cout << " " << location.try_files_.files[i];
    std::cout << '\n';
    std::cout << "fastcgi_pass:" << '\n';
    std::cout << "    address: " << location.fastcgi_pass_.address << '\n';
    std::cout << "    port: " << location.fastcgi_pass_.port << '\n';
    std::cout << "    unix: " << location.fastcgi_pass_.unix << '\n';
    std::cout << "\n";
}

int main(int argc, char *argv[])
{
    if (argc > 2) {
        std::cerr << "Usage: " << argv[0] << " [configuration file] | None" << '\n';
        return EXIT_FAILURE;
    }

    std::string file;
    if (argc == 1)
        file = "../../configs/default.conf";
    else
        file = argv[1];

    Config config(file);

    if (config.is_open() == false) {
        std::cerr << "Error: " << config.error_msg() << '\n';
        return EXIT_SUCCESS;
    }
    const BlockHttp_t http = config.http();
    const BlockServer_t server = config.server();

    print_http(http);
    print_server(server);

    // 존재하는 모든 서버를 벡터로 받아온다.
    /*
    const std::vector<BlockServer_t> servers = config.servers();
    for (size_t i = 0; i < servers.size(); i++)
        print_server(servers[i]);
    */

    // 존재하는 모든 location을 벡터로 받아온다.
    const std::vector<BlockLocation_t> locations = config.locations();
    for (size_t i = 0; i < locations.size(); i++)
        print_location(locations[i]);

    return EXIT_SUCCESS;
}
