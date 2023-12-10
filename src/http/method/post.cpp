#include <sys/stat.h>
#include "index.hpp"
#include "../http.hpp"
#include "../cgi.hpp"

static HttpStatus FileProcess(HttpRequest& req, HttpResponse& res, std::string path)
{
	// open file and write request body
	std::ofstream ofs(path.c_str(), std::ios::out);
	if (ofs.is_open() == false)
		return kInternalServerError;
	ofs << req.body().str();
	ofs.close();

	res.set_version(req.version());
	res.add_header("Content-Type", "text/html");
	res.add_header("Connection", "keep-alive");
	res.body() << "<html><body><h1>File created</h1></body></html>";
	res.set_done(true);
	return kCreated;
}

HttpStatus Http::Post(const location_t& location, const std::string url)
{
	std::string path(conf_.GetPath(url));

	std::cout << "location.name = " << location.name << std::endl;
	std::cout << "location.root = " << location.root << std::endl;
	std::cout << "location.fastcgi_param empty = " << location.fastcgi_param.empty() << std::endl;
	std::cout << "location.fastcgi_param cnt = " << location.fastcgi_param.size() << std::endl;
	if (location.fastcgi_param.empty()) {
		if (access(path.c_str(), F_OK) != -1) {
			std::cout << "Post: File already exists" << std::endl;
			return kSeeOther;
		}
		return FileProcess(request_, response_, path);
	}
	Multiplex::GetInstance().AddItem(new Cgi(conf_, url, request_, response_));
	return kCreated;
}
