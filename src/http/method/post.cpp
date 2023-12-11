#include <sys/stat.h>
#include "index.hpp"
#include "../http.hpp"
#include "../cgi.hpp"

static void FileProcess(HttpRequest& req, HttpResponse& res, std::string path)
{
	// open file and write request body
	std::ofstream ofs(path.c_str(), std::ios::out);
	if (ofs.is_open() == false)
		return res.set_status(kInternalServerError);
	ofs << req.body().str();
	ofs.close();

	res.set_version(req.version());
	res.add_header("Content-Type", "text/html");
	res.add_header("Connection", "keep-alive");
	res.body() << "<html><body><h1>File created</h1></body></html>";
	res.set_status(kCreated);
	res.set_done(true);
}

void Http::Post(const location_t& location, const std::string url)
{
	std::string path(conf_.GetPath(url));

	std::string cgi = Cgi::GetCgi(location, path);
	if (cgi.empty() == false) {
		Multiplex::GetInstance().AddItem(new Cgi(location, request_, response_));
		return;
	}
	struct stat sb;
	if (stat(path.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
		return response_.set_status(kForbidden);
	if (access(path.c_str(), F_OK) == -1)
		return FileProcess(request_, response_, path);
	response_.add_header("Location", request_.uri());
	response_.set_status(kSeeOther);
	response_.set_done(true);
}
