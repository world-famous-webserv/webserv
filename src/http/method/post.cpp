#include <sys/stat.h>
#include "../http.hpp"

HttpStatus Http::Post(const location_t& location)
{
    std::map<std::string, std::string>::const_iterator it = location.fastcgi_param.find("PATH_INFO");
    if (it == location.fastcgi_param.end()) {
        std::cout << "Post: PATH_INFO: empty" << '\n';
        return kNoContent;
    }

    std::string response_body = Cgi::run(request_, location, it->second);
    if (response_body.empty()) {
        std::cout << "Post: cgi.run: NoContent error" << '\n';
        return kNoContent;
    }
    if (response_body == "500") {
        std::cout << "Post: cgi.run: InternalServer error" << '\n';
        return kInternalServerError;
    }
    response_.body() << response_body;
	response_.set_version(request_.version());
	response_.add_header("Content-Type", "text/html");
	response_.add_header("Connection", "keep-alive");
	response_.set_status(kOk);
	response_.set_done(true);
	return kOk;
}
