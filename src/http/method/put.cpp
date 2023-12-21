#include "../http.hpp"

static void FileProcess(HttpRequest& req, HttpResponse& res, const location_t& location, std::string path, HttpStatus success_status)
{
	std::fstream file(path.c_str(), std::ios::out | std::ios::trunc);

	if (file.is_open() == false)
		return res.GenerateError(kInternalServerError, location.error_page);
	if (req.body().tellp() > 0)
		file << req.body().rdbuf();
	file.close();
	res.set_status(success_status);
	res.set_done(true);
}

void Http::Put(const location_t& location, const std::string url)
{
	(void)location;
	std::string path(conf_.GetPath(url));

	struct stat sb;
	if (stat(path.c_str(), &sb) == -1)
		return FileProcess(request_, response_, location, path, kCreated);
	if (S_ISDIR(sb.st_mode))
		return response_.GenerateError(kForbidden, location.error_page);
	return FileProcess(request_, response_, location, path, kNoContent);
}
