#include <sys/stat.h>
#include "index.hpp"
#include "../http.hpp"
#include "../cgi.hpp"

static void FileProcess(HttpRequest& req, HttpResponse& res, const location_t& location, std::string path)
{
	// open file and write request body
	std::ofstream ofs(path.c_str(), std::ios::out | std::ios::trunc);
	if (ofs.is_open() == false)
		return res.GenerateError(kInternalServerError, location.error_page);
	if (req.body().tellp() > 0)
		ofs << req.body().rdbuf();
	ofs.close();

	res.body() << "<html><body><h1>File created</h1></body></html>";
	res.set_status(kCreated);
	res.set_done(true);
}

void Http::Post(const location_t& location, const std::string url)
{
	std::string path(conf_.GetPath(url));

	std::string cgi = Cgi::GetCgi(location, path);
	if (cgi.empty() == false) {
		if (access(cgi.c_str(), F_OK) == -1
			|| access(cgi.c_str(), X_OK) == -1)
			return response_.GenerateError(kInternalServerError, location.error_page);
		Multiplex::GetInstance().AddItem(new Cgi(location, request_, response_));
		return;
	}

	struct stat sb;
	if (stat(path.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
		return response_.GenerateError(kInternalServerError, location.error_page);
	return FileProcess(request_, response_, location, path);
}
