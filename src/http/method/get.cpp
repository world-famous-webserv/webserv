#include <sys/stat.h>
#include "../http.hpp"

static HttpStatus FileProcess(const HttpRequest& req, HttpResponse& res, std::string path)
{
	std::fstream file(path.c_str());

	if (file.is_open() == false)
		return kForbidden;
	res.set_status(kOk);
	res.set_version(req.version());
	res.add_header("Content-Type", "text/html");
	res.add_header("Connection", "keep-alive");
	res.body() << file.rdbuf();
	res.set_done(true);
	file.close();
	return kOk;
}

static HttpStatus CgiProcess(const HttpRequest& req, HttpResponse& res, const location_t &location, std::string path)
{
	(void)req;
	(void)res;
	(void)location;
	(void)path;
	return kForbidden;
}
#include "index.hpp"
static HttpStatus DirectoryProcess(const HttpRequest& req, HttpResponse& res, const location_t &location, std::string path)
{
	// index
	if (index(req, res, location, path) == EXIT_SUCCESS)
		return kOk;
	// autoindex
	if (location.autoindex == false)
		return kForbidden;
	autoindex(req, res, path);
	return kOk;
}

HttpStatus Http::Get(const location_t& location, const std::string url)
{
	std::string path(conf_.GetPath(url));

	struct stat sb;
	if (stat(path.c_str(), &sb) == -1)
		return kForbidden;
	if (S_ISDIR(sb.st_mode))
		return DirectoryProcess(request_, response_, location, path);
	if (!S_ISREG(sb.st_mode))
		return kForbidden;
	
	bool is_cgi = false;
#if 1
	if (is_cgi)
		return CgiProcess(request_, response_, location, path);
#endif
	return FileProcess(request_, response_, path);
}
