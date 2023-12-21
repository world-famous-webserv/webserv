#include <sys/stat.h>
#include "index.hpp"
#include "../http.hpp"
#include "../cgi.hpp"

static void FileProcess(const HttpRequest& req, HttpResponse& res, const location_t& location, std::string path)
{
	(void)req;
	std::fstream file(path.c_str());

	if (file.is_open() == false)
		return res.GenerateError(kForbidden, location.error_page);
	res.body() << file.rdbuf();
	file.close();
	res.set_done(true);
}

static void DirectoryProcess(const HttpRequest& req, HttpResponse& res, const location_t &location, std::string path)
{
	// index
	if (index(req, res, location, path) == EXIT_SUCCESS)
		return;
	// autoindex
	if (location.autoindex == false)
		return res.GenerateError(kNotFound, location.error_page);
	autoindex(req, res, path);
}

void Http::Get(const location_t& location, const std::string url)
{
	std::string path(conf_.GetPath(url));

	struct stat sb;
	if (stat(path.c_str(), &sb) == -1)
		return response_.GenerateError(kNotFound, location.error_page);
	if (S_ISDIR(sb.st_mode))
		return DirectoryProcess(request_, response_, location, path);
	std::string cgi = Cgi::GetCgi(location, path);
	if (cgi.empty())
		return FileProcess(request_, response_, location, path);
	if (access(cgi.c_str(), F_OK) == -1
		|| access(cgi.c_str(), X_OK) == -1)
		return response_.GenerateError(kInternalServerError, location.error_page);
	Multiplex::GetInstance().AddItem(new Cgi(location, request_, response_));
}
