#include <sys/stat.h>
#include "index.hpp"
#include "../http.hpp"
#include "../cgi.hpp"

static void FileProcess(const HttpRequest& req, HttpResponse& res, std::string path)
{
	std::fstream file(path.c_str());

	if (file.is_open() == false)
		return res.set_status(kForbidden);
	res.body() << file.rdbuf();
	file.close();
	res.set_status(kOk);
	res.set_version(req.version());
	res.add_header("Content-Type", "text/html");
	res.add_header("Connection", "keep-alive");
	res.set_done(true);
}

static void DirectoryProcess(const HttpRequest& req, HttpResponse& res, const location_t &location, std::string path)
{
	// index
	if (index(req, res, location, path) == EXIT_SUCCESS)
		return res.set_status(kOk);
	// autoindex
	if (location.autoindex == false)
		return res.set_status(kNotFound);
	autoindex(req, res, path);
	return res.set_status(kOk);
}

void Http::Get(const location_t& location, const std::string url)
{
	std::string path(conf_.GetPath(url));

	struct stat sb;
	if (stat(path.c_str(), &sb) == -1)
		return response_.set_status(kNotFound);
	if (S_ISDIR(sb.st_mode))
		return DirectoryProcess(request_, response_, location, path);
	std::string cgi = Cgi::GetCgi(location, path);
	if (cgi.empty())
		return FileProcess(request_, response_, path);
	Multiplex::GetInstance().AddItem(new Cgi(location, request_, response_));
}
