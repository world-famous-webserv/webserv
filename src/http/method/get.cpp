#include <sys/stat.h>
#include "index.hpp"
#include "../http.hpp"
#include "../cgi.hpp"

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

	std::cout << "location.name = " << location.name << std::endl;
	std::cout << "location.root = " << location.root << std::endl;
	std::cout << "location.fastcgi_param empty = " << location.fastcgi_param.empty() << std::endl;
	std::cout << "location.fastcgi_param cnt = " << location.fastcgi_param.size() << std::endl;
	if (location.fastcgi_pass.empty())
		return FileProcess(request_, response_, path);
	Multiplex::GetInstance().AddItem(new Cgi(conf_, url, request_, response_));
	return kOk;
}
