#include "conf.hpp"

Conf::~Conf() { }
Conf::Conf(const server_t &server, const listen_t &listen):
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
    client_header_timeout(server.client_header_timeout),
    client_header_buffer_size(server.client_header_buffer_size),
    keepalive_requests(server.keepalive_requests),
    keepalive_time(server.keepalive_time),
    keepalive_timeout(server.keepalive_timeout),
    index(server.index),
    server_name(server.server_name),
    locations(server.locations),
    error_page(server.error_page),
    fastcgi_param(server.fastcgi_param),
    try_files(server.try_files),
    ret(server.ret),
    listen(listen)
{ }

Conf::Conf(const Conf &other):
    sendfile(other.sendfile),
    autoindex(other.autoindex),
    absolute_redirect(other.absolute_redirect),
    server_name_in_redirect(other.server_name_in_redirect),
    tcp_nodelay(other.tcp_nodelay),
    tcp_nopush(other.tcp_nopush),
    default_type(other.default_type),
    root(other.root),
    allows(other.allows),
    denys(other.denys),
    linger(other.linger),
    send_lowat(other.send_lowat),
    sendfile_max_chunk(other.sendfile_max_chunk),
    client_max_body_size(other.client_max_body_size),
    client_body_timeout(other.client_body_timeout),
    client_header_timeout(other.client_header_timeout),
    client_header_buffer_size(other.client_header_buffer_size),
    keepalive_requests(other.keepalive_requests),
    keepalive_time(other.keepalive_time),
    keepalive_timeout(other.keepalive_timeout),
    index(other.index),
    server_name(other.server_name),
    locations(other.locations),
    error_page(other.error_page),
    fastcgi_param(other.fastcgi_param),
    try_files(other.try_files),
    ret(other.ret),
    listen(other.listen)
{ }

Conf &Conf::operator=(const Conf &other)
{
    if (this == &other)
        return *this;
    sendfile = other.sendfile;
    autoindex = other.autoindex;
    absolute_redirect = other.absolute_redirect;
    server_name_in_redirect = other.server_name_in_redirect;
    tcp_nodelay = other.tcp_nodelay;
    tcp_nopush = other.tcp_nopush;
    default_type = other.default_type;
    root = other.root;
    allows = other.allows;
    denys = other.denys;
    linger = other.linger;
    send_lowat = other.send_lowat;
    sendfile_max_chunk = other.sendfile_max_chunk;
    client_max_body_size = other.client_max_body_size;
    client_body_timeout = other.client_body_timeout;
    client_header_timeout = other.client_header_timeout;
    client_header_buffer_size = other.client_header_buffer_size;
    keepalive_requests = other.keepalive_requests;
    keepalive_time = other.keepalive_time;
    keepalive_timeout = other.keepalive_timeout;
    index = other.index;
    server_name = other.server_name;
    locations = other.locations;
    error_page = other.error_page;
    fastcgi_param = other.fastcgi_param;
    try_files = other.try_files;
    ret = other.ret;
    listen = other.listen;
    return *this;
}

std::string Conf::GetUrl(const std::string &str) const
{
    std::vector<std::string> parts;
    std::istringstream iss(str);

    if (str == "/")
        return "/";
        
    for (std::string part; std::getline(iss, part, '/');) {
		if (part == "" || part == ".")
            continue;
		if (part == "..") {
			if (parts.empty())
				return "";
			parts.pop_back();
		}
        else
			parts.push_back(part);
	}
    if (parts.empty())
        return "";
    std::ostringstream oss;
    for (std::vector<std::string>::const_iterator i = parts.begin(), end = parts.end(); i != end; ++i)
        oss << "/" << *i;
    return oss.str();
}

std::string Conf::GetPath(const std::string &url) const
{
    const int idx = GetLocationIdx(url);
    if (idx == -1)
        return "";
    const location_t &location = GetLocation(idx);
    const std::string path = location.root + url.substr(location.name.length() - (location.name[0] == '/'));
    return path;
}

int Conf::GetLocationIdx(const std::string &url) const
{
    size_t max_location_name_length = 0;

    int idx = -1;
	for (size_t i = 0, end = locations.size(); i != end; ++i) {
		const location_t &location = locations[i];
		if (url.length() > max_location_name_length && url.length() >= location.name.length() && url.compare(0, location.name.length(), location.name) == 0) {
			max_location_name_length = location.name.length();
            idx = (int)i;
		}
	}
    return idx;
}

location_t Conf::GetLocation(const int idx) const
{
    return locations[idx];
}
