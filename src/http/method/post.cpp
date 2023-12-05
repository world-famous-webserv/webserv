#include <sys/stat.h>
#include "../http.hpp"

HttpStatus Http::Post(const location_t& location, const std::string url)
{
    (void)location;
	const std::string path = conf_.GetPath(url);

	struct stat sb;
	if (stat(path.c_str(), &sb) == -1) {
        std::cout << "Post: stat: [" + path + " ] stat error" << '\n';
        return kForbidden;
    }
	// if (S_ISDIR(sb.st_mode) == false) {
    //     std::cout << "Post: S_ISDIR: [" + path + " ] error" << '\n';
    //     return kForbidden;
    // }

    if (location.cgi_pass.empty()) {
        std::cout << "Post: cgi_pass: [" + path + " ] cgi_pass expty error" << '\n';
        return kNoContent;
    }
    std::cout << "Bofore Cgi: " << request_.body().str() << '\n';

    Cgi cgi(request_, location);
    std::string response_body = cgi.run(location.cgi_pass);
    if (response_body.empty()) {
        std::cout << "Post: cgi.run: [" + path + " ] NoContent error" << '\n';
        return kNoContent;
    }
    if (response_body == "500") {
        std::cout << "Post: cgi.run: [" + path + " ] InternalServer error" << '\n';
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
