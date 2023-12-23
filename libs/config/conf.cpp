#include "conf.hpp"

Conf::~Conf() { }
Conf::Conf(const server_t &server, const listen_t &listen):
    linger(server.linger),
    server_name(server.server_name),
    locations(server.locations),
    error_page(server.error_page),
    listen(listen)
{
}

Conf::Conf(const Conf &other):
    linger(other.linger),
    server_name(other.server_name),
    locations(other.locations),
    error_page(other.error_page),
    listen(other.listen)
{ }

Conf &Conf::operator=(const Conf &other)
{
    if (this == &other)
        return *this;
    linger = other.linger;
    server_name = other.server_name;
    locations = other.locations;
    error_page = other.error_page;
    listen = other.listen;
    return *this;
}

std::string Conf::GetUrl(const std::string &str) const
{
    const bool is_front_slash = str[0] == '/';
    const bool is_back_slash = str[str.length() - 1] == '/';

    std::vector<std::string> parts;
    std::istringstream iss(str);
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
    std::stringstream oss;
    if (is_front_slash)
        oss << '/';
    for (std::vector<std::string>::const_iterator it = parts.begin(), end = parts.end(); it != end; ++it)
        oss << *it << '/';
    std::string url = oss.str();
    if (is_back_slash == false)
        url = url.substr(0, url.length() - 1);
    return url;
}

std::string Conf::GetPath(const std::string &url) const
{
    const int idx = GetLocationIdx(url);
    if (idx == -1)
        return "";
    const location_t &location = GetLocation(idx);
    const std::string path = location.root + url.substr(location.name.length(), url.length() - location.name.length());
    return path;
}

std::string Conf::GetExt(const std::string &url) const
{
    const std::string ext = url.substr(url.find_last_of('.') + 1);
    return ext;
}

int Conf::GetLocationIdx(const std::string &url) const
{
    size_t max_location_name_length = 0;

    int idx = -1;
	for (size_t i = 0, end = locations.size(); i != end; ++i) {
		const location_t &location = locations[i];
		if (url.length() > max_location_name_length && \
            url.length() >= location.name.length() && \
            url.compare(0, location.name.length(), location.name) == 0)
        {
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
