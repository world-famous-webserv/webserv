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

	struct stat sb;
	std::cerr << "Post: " << path << std::endl;
	if (stat(path.c_str(), &sb) != -1) {
		if (S_ISDIR(sb.st_mode))
			return kForbidden;
		if (access(path.c_str(), F_OK) != -1) {
			std::cerr << "Post: File already exists" << std::endl;
			return kNoContent;
		}
		return kInternalServerError;
	}
	if (location.fastcgi_pass.empty())
		return FileProcess(request_, response_, path);
	const std::string &cgipass = Cgi::GetCgiPass(location, path);
	if (cgipass.empty()) {
		response_.set_status(kNotFound);
		return kNotFound;
	}
	Multiplex::GetInstance().AddItem(new Cgi(cgipass, request_, response_));
	return kCreated;
}
