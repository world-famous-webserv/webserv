#include <sys/stat.h>
#include "../http.hpp"

HttpStatus Http::Post(const location_t& location, const std::string url)
{
    (void)location;
	const std::string path = conf_.GetPath(url);

	// struct stat sb;
	// if (stat(path.c_str(), &sb) == -1) {
    //     std::cout << "Post: stat: [" + path + " ] error" << '\n';
    //     return kForbidden;
    // }
	// if (S_ISDIR(sb.st_mode) == false) {
    //     std::cout << "Post: S_ISDIR: [" + path + " ] error" << '\n';
    //     return kForbidden;
    // }

	response_.set_status(kOk);
	response_.set_version(request_.version());
	response_.add_header("Content-Type", "text/html");
	response_.add_header("Connection", "keep-alive");
	response_.body()
        << "<html>"
        << "<head><title>POST</title></head>"
        << "<body>"
        << "<h1>POST</h1>"
        << "<p>BODY: " << request_.body().rdbuf() << "</p>"
        << "</body>"
        << "</html>";
	response_.set_done(true);
	return kOk;
}
