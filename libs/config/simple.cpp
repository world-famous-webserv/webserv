#include "simple.hpp"

Simple::~Simple() { }
Simple::Simple() { }
Simple::Simple(const Simple &obj) { *this = obj; }
Simple &Simple::operator=(const Simple &obj)
{
    if (this == &obj)
        return *this;
    return *this;
}

void Simple::InitKeepaliveTimeout(keepalive_timeout_t &keepalive_timeout)
{
    keepalive_timeout.timeout = 75;
    keepalive_timeout.header_timeout = 0;
}

void Simple::InitListen(listen_t &listen)
{
    listen.address = "*";
    listen.port = 8000;
    listen.default_server = false;
    listen.ssl = false;
    listen.http2 = false;
    listen.quic = false;
    listen.proxy_protocol = false;
    listen.deferred = false;
    listen.bind = false;
    listen.ipv6only = false;
    listen.reuseport = false;
    listen.so_keepalive = false;
    listen.keepidle = 0;
    listen.keepintvl = 0;
    listen.keepcnt = 0;
    listen.setfib = 0;
    listen.fastopen = 0;
    listen.backlog = 0;
    listen.rcvbuf = 0;
    listen.sndbuf = 0;
    listen.accept_filter = "";
    listen.unixpath = "";
}

void Simple::InitFastcgiPass(fastcgi_pass_t &fastcgi_pass)
{
    fastcgi_pass.address = "";
    fastcgi_pass.port = 0;
    fastcgi_pass.unix = false;
}

void Simple::InitTryFiles(try_files_t &try_files)
{
    try_files.files.clear();
    try_files.uri = "";
    try_files.code = 0;
}

bool Simple::ParseBoolType(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg_, const std::string &directive)
{
    bool res = false;
    if (idx == tokens.size()) {
        error_msg_ =  directive + ": Missing argument";
        return res;
    }
    if (tokens[idx] == "on")
        res = true;
    else if (tokens[idx] == "off")
        res = false;
    else {
        error_msg_ = directive + ": Invalid argument [ " + tokens[idx] + " ]";
        return res;
    }
    ++idx;
    if (idx == tokens.size() || tokens[idx] != ";")
        error_msg_ = directive + ": Missing ;";
    else
        ++idx;
    return res;
}

std::string Simple::ParseStringType(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg_, const std::string &directive)
{
    std::string res;
    if (idx == tokens.size()) {
        error_msg_ = directive + ": Missing argument";
        return res;
    }
    res = tokens[idx++];
    if (idx == tokens.size() || tokens[idx] != ";")
        error_msg_ = directive + ": Missing ;";
    else
        ++idx;
    return res;
}

keepalive_timeout_t Simple::ParseKeepaliveTimeout(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg_)
{
    keepalive_timeout_t keepalive_timeout; InitKeepaliveTimeout(keepalive_timeout);
    if (idx == tokens.size()) {
        error_msg_ =  "keepalive_timeout: Missing argument";
        return keepalive_timeout;
    }
    const int timeout = Utils::StoiTime(tokens[idx]);
    if (timeout == -1) {
        error_msg_ = "keepalive_timeout: Invalid argument";
        return keepalive_timeout;
    }
    keepalive_timeout.timeout = static_cast<size_t>(timeout);
    ++idx;
    if (idx == tokens.size()) {
        error_msg_ = "keepalive_timeout: Missing ;";
        return keepalive_timeout;
    }
    if (tokens[idx] == ";") {
        ++idx;
        return keepalive_timeout;
    }
    const int header_timeout = Utils::StoiTime(tokens[idx]);
    if (header_timeout == -1) {
        error_msg_ = "keepalive_timeout: header_timeout: Invalid argument";
        return keepalive_timeout;
    }
    keepalive_timeout.header_timeout = static_cast<size_t>(header_timeout);
    ++idx;
    if (idx == tokens.size() || tokens[idx] != ";")
        error_msg_ = "keepalive_timeout: Missing ;";
    else
        ++idx;
    return keepalive_timeout;
}

listen_t Simple::ParseListen(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg_)
{
    listen_t listen; InitListen(listen);
    if (idx == tokens.size()) {
        error_msg_ =  "listen: Missing argument";
        return listen;
    }
    const std::string argument = tokens[idx];
    size_t pos = argument.find(":");

    if (pos == std::string::npos) {
        if (argument.find_first_not_of("0123456789") == std::string::npos)
            listen.port = static_cast<size_t>(std::stoi(argument));
        else {
            listen.address = argument;
            listen.port = 80;
        }
    } else if (argument.substr(0, pos) == "unix") {
        if (pos + 1 == argument.size()) {
            error_msg_ = "listen: Missing unix path";
            return listen;
        }
        listen.unixpath = argument.substr(pos + 1);
    } else {
        listen.address = argument.substr(0, pos);
        if (pos + 1 == argument.size()) {
            error_msg_ = "listen: Missing port";
            return listen;
        }
        const std::string port_s = argument.substr(pos + 1);
        if (port_s.find_first_not_of("0123456789") == std::string::npos) {
            listen.port = static_cast<size_t>(std::stoi(port_s));
        } else {
            error_msg_ = "listen: Invalid port";
            return listen;
        }
    }
    ++idx;

    while (idx < tokens.size() && tokens[idx] != ";") {
        const std::string option = tokens[idx];
        if (option == "default_server")
            listen.default_server = true;
        else if (option == "ssl")
            listen.ssl = true;
        else if (option == "http2") {
            if (listen.quic == true) {
                error_msg_ = "listen: http2: http2 and quic are mutually exclusive";
                return listen;
            }
            listen.http2 = true;
        }
        else if (option == "quic") {
            if (listen.http2 == true) {
                error_msg_ = "listen: quic: http2 and quic are mutually exclusive";
                return listen;
            }
            listen.quic = true;
        }
        else if (option == "proxy_protocol")
            listen.proxy_protocol = true;
        else if (option == "deferred")
            listen.deferred = true;
        else if (option == "bind")
            listen.bind = true;
        else if (option == "ipv6only")
            listen.ipv6only = true;
        else if (option == "reuseport")
            listen.reuseport = true;
        else if (option.substr(0, 12) == "so_keepalive") {
            if (option.size() < 14 || option[12] != '=') {
                error_msg_ = "listen: Missing so_keepalive value";
                return listen;
            }
            const std::string value = option.substr(13);
            if (value == "on")
                listen.so_keepalive = true;
            else if (value == "off")
                listen.so_keepalive = false;
            else {
                const size_t pos1 = value.find_first_of(":");
                const size_t pos2 = value.find_last_of(":");
                if (pos1 == std::string::npos || pos1 == pos2) {
                    error_msg_ = "listen: so_keepalive: Invalid value";
                    return listen;
                }
                listen.keepidle = static_cast<size_t>(std::stoi(value.substr(0, pos1)));
                listen.keepintvl = static_cast<size_t>(std::stoi(value.substr(pos1 + 1, pos2 - pos1 - 1)));
                listen.keepcnt = static_cast<size_t>(std::stoi(value.substr(pos2 + 1)));
            }
        }
        else if (option.substr(0, 6) == "setfib") {
            if (option.size() < 8 || option[6] != '=') {
                error_msg_ = "listen: setfib: Missing number";
                return listen;
            }
            listen.setfib = std::stoi(option.substr(7));
        } else if (option.substr(0, 8) == "fastopen") {
            if (option.size() < 10 || option[8] != '=') {
                error_msg_ = "listen: fastopen: Missing number";
                return listen;
            }
            listen.setfib = std::stoi(option.substr(9));
        } else if (option.substr(0, 7) == "backlog") {
            if (option.size() < 9 || option[7] != '=') {
                error_msg_ = "listen: backlog: Missing number";
                return listen;
            }
            listen.backlog = std::stoi(option.substr(8));
        } else if (option.substr(0, 6) == "rcvbuf") {
            if (option.size() < 8 || option[6] != '=') {
                error_msg_ = "listen: rcvbuf: Missing number";
                return listen;
            }
            listen.rcvbuf = static_cast<size_t>(std::stoi(option.substr(7)));
        } else if (option.substr(0, 6) == "sndbuf") {
            if (option.size() < 8 || option[6] != '=') {
                error_msg_ = "listen: sndbuf: Missing number";
                return listen;
            }
            listen.sndbuf = static_cast<size_t>(std::stoi(option.substr(7)));
        } else if (option.substr(0, 13) == "accept_filter") {
            if (option.size() < 15 || option[13] != '=') {
                error_msg_ = "listen: accept_filter: Missing filter value";
                return listen;
            }
            listen.accept_filter = option.substr(14);
            if (listen.accept_filter != "dataready" && listen.accept_filter != "httpready") {
                error_msg_ = "listen: accept_filter: Invalid filter value";
                return listen;
            }
        } else {
            error_msg_ = "listen: Unknown option [ " + option + " ]";
            return listen;
        }
        ++idx;
    }
    if (idx == tokens.size())
        error_msg_ = "listen: Missing ;";
    else
        ++idx;
    return listen;
}

std::vector<std::string> Simple::ParseStringVectorType(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg_, const std::string &directive)
{
    std::vector<std::string> res;
    if (idx == tokens.size()) {
        error_msg_ =  directive + ": Missing argument";
        return res;
    }
    while (idx < tokens.size() && tokens[idx] != ";")
        res.push_back(tokens[idx++]);
    if (idx == tokens.size())
        error_msg_ = directive + ": Missing ;";
    else
        ++idx;
    return res;
}

size_t Simple::ParseSizeType(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg_, const std::string &directive, const bool is_time)
{
    size_t res = 0;
    if (idx == tokens.size()) {
        error_msg_ = directive + ": Missing argument";
        return res;
    }
    const std::string argument = tokens[idx++];
    if (argument == "unlimited")
        return res;
    const size_t pos = argument.find_first_not_of("0123456789");
    if (pos == std::string::npos)
        res = static_cast<size_t>(std::stoi(argument));
    else {
        res = static_cast<size_t>(std::stoi(argument.substr(0, pos)));
        const std::string unit = argument.substr(pos);
        if (is_time) {
            if (unit == "s" || unit == "S")
                res *= 1;
            else if (unit == "m" || unit == "M")
                res *= 60;
            else if (unit == "h" || unit == "H")
                res *= 60 * 60;
            else if (unit == "d" || unit == "D")
                res *= 60 * 60 * 24;
            else if (unit == "w" || unit == "W")
                res *= 60 * 60 * 24 * 7;
            else if (unit == "M")
                res *= 60 * 60 * 24 * 30;
            else if (unit == "y" || unit == "Y")
                res *= 60 * 60 * 24 * 365;
            else {
                error_msg_ = directive + ": Invalid Unit";
                return res;
            }
        } else {
            if (unit == "k" || unit == "K")
                res *= 1024;
            else if (unit == "m" || unit == "M")
                res *= 1024 * 1024;
            else if (unit == "g" || unit == "G")
                res *= 1024 * 1024 * 1024;
            else {
                error_msg_ = directive + ": Invalid Unit";
                return res;
            }
        }
    }
    if (idx == tokens.size() || tokens[idx] != ";")
        error_msg_ = directive + ": Missing ;";
    else
        ++idx;
    return res;
}

std::map<int, std::string> Simple::ParseErrorPage(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg_)
{
    std::map<int, std::string> error_page;

    if (idx == tokens.size()) {
        error_msg_ = "error_page: Missing status code";
        return error_page;
    }
    if (idx + 1 == tokens.size()) {
        error_msg_ = "error_page: Missing uri";
        return error_page;
    }

    std::vector<std::string> arguments;

    while (idx < tokens.size() && tokens[idx] != ";")
        arguments.push_back(tokens[idx++]);
    if (idx == tokens.size()) {
        error_msg_ = "error_page: Missing ;";
        return error_page;
    }
    else
        ++idx;

    std::string uri = arguments.back(); arguments.pop_back();
    for (size_t i = 0; i < arguments.size(); ++i) {
        const std::string argument = arguments[i];
        const size_t pos = argument.find_first_not_of("0123456789");
        if (pos != std::string::npos) {
            error_msg_ = "error_page: status code: Invalid character argument: " + argument + " at: " + std::to_string(pos);
            return error_page;
        }
        const int status_code = std::stoi(argument);
        if (status_code < 100 || status_code > 599) {
            error_msg_ = "error_page: status code: Invalid range";
            return error_page;
        }
        error_page[status_code] = uri;
    }
    return error_page;
}

try_files_t Simple::ParseTryFiles(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg_)
{
    try_files_t try_files; InitTryFiles(try_files);

    if (idx == tokens.size()) {
        error_msg_ = "try_files: Missing file";
        return try_files;
    }
    if (idx + 1 == tokens.size()) {
        error_msg_ = "try_files: Missing uri or code";
        return try_files;
    }

    std::vector<std::string> arguments;

    while (idx < tokens.size() && tokens[idx] != ";")
        arguments.push_back(tokens[idx++]);
    if (idx == tokens.size()) {
        error_msg_ = "try_files: Missing ;";
        return try_files;
    }
    else
        ++idx;

    std::string token = arguments.back(); arguments.pop_back();

    if (token[0] == '=') {
        std::string code_s = token.substr(1);
        size_t pos = code_s.find_first_not_of("0123456789");
        if (pos != std::string::npos) {
            error_msg_ = "try_files: code: Invalid number";
            return try_files;
        } else
            try_files.code = static_cast<size_t>(std::stoi(code_s));
    } else
        try_files.uri = token;
    try_files.files = arguments;
    return try_files;
}

void Simple::ParseFastcgiParam(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg_, std::map<std::string, std::string> &fastcgi_param)
{
    if (idx == tokens.size()) {
        error_msg_ = "fastcgi_param: Missing parameter";
        return;
    }
    if (idx + 1 == tokens.size()) {
        error_msg_ = "fastcgi_param: Missing value";
        return;
    }
    if (idx + 2 == tokens.size() || tokens[idx + 2] != ";") {
        error_msg_ = "fastcgi_param: Missing ;";
        return;
    }
    const std::string parameter = tokens[idx++];
    const std::string value = tokens[idx++];
    fastcgi_param[parameter] = value;
    ++idx;
    return;
}

fastcgi_pass_t Simple::ParseFastcgiPass(const std::vector<std::string> &tokens, size_t &idx, std::string &error_msg_)
{
    fastcgi_pass_t fastcgi_pass; InitFastcgiPass(fastcgi_pass);
    if (idx == tokens.size()) {
        error_msg_ = "fastcgi_pass: Missing argument";
        return fastcgi_pass;
    }
    const std::string argument = tokens[idx++];
    size_t pos = argument.find_first_of(":");
    if (pos == std::string::npos) {
        fastcgi_pass.address = argument;
        fastcgi_pass.port = 8000;
    } else {
        fastcgi_pass.address = argument.substr(0, pos);
        const std::string port_s = argument.substr(pos + 1);
        if (port_s.find_first_not_of("0123456789") == std::string::npos)
            fastcgi_pass.port = static_cast<size_t>(std::stoi(port_s));
        else {
            error_msg_ = "fastcgi_pass: Invalid port";
            return fastcgi_pass;
        }
    }
    if (idx == tokens.size() || tokens[idx] != ";")
        error_msg_ = "fastcgi_pass: Missing ;";
    else
        ++idx;
    return fastcgi_pass;
}
