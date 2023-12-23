#include "index.hpp"

std::string	setHead(std::string & path);
std::string setAtagLine(std::string filename, char * time,  off_t filesize, bool is_dir);
std::string	setAtag(std::string pathname, std::string filename);
std::string	setEnd();

bool	index(const HttpRequest& req, HttpResponse& res, const location_t &location, std::string path){
	(void)req;
	std::vector<std::string> index = location.index;
	
	for (size_t i = 0; i < index.size(); ++i) {
		std::string tmp = path + std::string("/") + index[i];
		if (access(tmp.c_str(), F_OK) == -1)
			continue;
		std::fstream file(tmp.c_str());
		if (file.is_open() == false) 
			continue;
		res.body() << file.rdbuf();
		file.close();
		res.set_done(true);
		return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}

std::string	setHead(const std::string & path) {
	std::stringstream ss;

	ss << "<html><head>";
	ss << "<title>Index of " + path + "</title>";
	if (path[path.length() - 1] != '/')
		ss << "<base href=\"" + path + "/\">";
	ss << "</head>\n<body>\n<h1>Index of " + path + "</h1><hr><pre>\n";
	return ss.str();
}

std::string setAtagLine(std::string filename, char * time,  off_t filesize, bool is_dir) {
	std::stringstream ss;
	std::string line;

	if (is_dir) 
		ss << "<a href=\"" + filename + "/\">" 
			<< std::left << std::setw(50) << filename + "/</a>" 
			<< std::setw(25) << time 
			<< std::right << std::setw(10) << "-" << " \r\n";
	else
		ss << "<a href=\"" + filename + "\">" 
			<< std::left << std::setw(50) << filename + "</a>" 
			<< std::setw(25) << time 
			<< std::right << std::setw(10) << filesize << " \r\n";
	return ss.str();
}

std::string	setAtag(std::string pathname, std::string filename) {
	struct stat	sb;
	std::string	line;

	stat(pathname.c_str(), &sb);
	std::tm *tm = std::localtime(&sb.st_mtime);
	char mbstr[100];
	std::strftime(mbstr, sizeof(mbstr), "%d-%b-%Y %H:%M", tm);
	line += setAtagLine(filename, mbstr, sb.st_size, S_ISDIR(sb.st_mode));
	return line;
}

std::string	setEnd() {
	return "</pre><hr>\n\n</body></html>";
}

void	autoindex(const HttpRequest& req, HttpResponse& res, std::string & path) {
	struct dirent	*entry;
	DIR 			*dp = opendir(path.c_str());

	res.body() << setHead(req.uri());
	while ((entry = readdir(dp)) != NULL) {
		std::string dname(entry->d_name);
		if (dname == ".")
			continue ;
		if (dname == "..") {
			res.body() << "<a href=\"../\">../</a>\r\n";
			continue ;
		}
		res.body() << setAtag(path + std::string("/") + dname, dname);
	}
	res.body() << setEnd();
	closedir(dp);
	res.set_done(true);
}
