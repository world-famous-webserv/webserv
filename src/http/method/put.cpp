#include "../http.hpp"

static HttpStatus FileProcess(HttpRequest& req, HttpResponse& res, std::string path, HttpStatus success_status)
{
	std::fstream file(path.c_str(), std::ios::out | std::ios::trunc);

	if (file.is_open() == false)
		return kInternalServerError;
	file << req.body().str();
	file.close();
	res.set_version(req.version());
	res.add_header("Connection", "keep-alive");
	res.set_done(true);
	return success_status;
}

HttpStatus Http::Put(const location_t& location, const std::string url)
{
	(void)location;
	std::string path(conf_.GetPath(url));

	struct stat sb;
	if (stat(path.c_str(), &sb) == -1)
		return FileProcess(request_, response_, path, kCreated);
	if (S_ISDIR(sb.st_mode))
		return kForbidden;
	return FileProcess(request_, response_, path, kNoContent);
}
