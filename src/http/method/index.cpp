#include "index.hpp"

bool hasAccess(std::string & fileName, int mode) {
	return (access(fileName.c_str(), mode) == 0); // 파일에 대한 접근 권한을 확인하여 반환
}

bool	index(){
	/// 지워야 하는 부분
	// config 에서  index.html 이 "index.htm" "index.ht" "index.h" 로 루트된 경우
	std::string path = "index.html"; 
	std::string paths[4] = {"/Users/seojo/Desktop/webserv/index.htm", 
							"/Users/seojo/Desktop/webserv/index.ht", 
							"/Users/seojo/Desktop/webserv/index.h"};
	std::string BODY;
	int			http_status = 0;
	//////

	int	i = 0;
	while(!paths[i].empty()) {
		if (hasAccess(paths[i], F_OK) == 0){ // 파일 존재여부 확인.
			++i;
			continue;
		}
		else{
			std::fstream file(paths[i]);
			if (file.is_open()) {
				std::string line;
				while (std::getline(file, line))
					BODY += line + "\n";  // << body 에 입력
					http_status = kOk;  // <<< status 변경
					file.close();
					return EXIT_SUCCESS;
			}
			else{
				if (hasAccess(paths[i], R_OK) == 0) // 파일 읽기권한 확인
					http_status = kForbidden; // <<< status 변경
				return EXIT_FAILURE; ///!@%@#% 다른 파일은 열린다면????
			}
		}
	}
	return EXIT_FAILURE;
}

bool	is_dir(std::string & path)
{
	struct stat	sb;

	if (stat(path.c_str(), &sb) == -1)
		throw std::runtime_error(strerror(errno));
	if ((sb.st_mode & S_IFMT) == S_IFDIR)
		return true;
	return false;
}

std::string	setHead(std::string & path) {
	return "<html><head><title>Index of " + path + "</title></head>\n<body>\n<h1>Index of " + path + "</h1><hr><pre>\n";
}

std::string setAtagLine(std::string filename, char * time,  off_t filesize = 0) {
	std::stringstream ss;
	std::string line;

//	if (is_dir(filename)) 
	if (filesize == 0) 
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

	std::cout << "pathname: " + pathname << "\n";
	std::cout << "filename: " + filename << "\n";
	if (stat(pathname.c_str(), &sb) == -1) // << 절대경로를 사용해야 함.
	{
		std::cout << "here\n";
		throw std::runtime_error(strerror(errno));
	}
	
	char mbstr[100];
	std::strftime(mbstr, sizeof(mbstr), "%d-%b-%Y %H:%M", std::localtime(&sb.st_mtime));

	if ((sb.st_mode & S_IFMT) == S_IFDIR)
		line += setAtagLine(filename, mbstr);
	else
		line += setAtagLine(filename, mbstr, sb.st_size);
	return line;
}

std::string	setEnd() {
	return "</pre><hr>\n\n</body></html>";
}

std::string	setHtml(std::string & path) {
	struct dirent	*entry;
	DIR 			*dp = opendir(path.c_str());
	std::string		BODY;

	std::cout << "setHtml Path: " << path <<"\n";
	BODY += setHead(path);
	while ((entry = readdir(dp)) != NULL) {
		std::string dname(entry->d_name);
		if (dname == ".")
			continue ;
		if (dname == "..") {
			BODY += "<a href=\"../\">../</a>\r\n";
			continue ;
		}
		std::cout << "setAtag path: " << entry->d_name << "\n";
		BODY += setAtag(path + std::string("/") + dname, dname); // << /  이슈
	}
	BODY += setEnd();
	closedir(dp);
	return BODY;
}

std::string	autoindex(std::string path)
{
	/// 지울부분
	int			http_status = 0;
	bool		autoindex = true;
	/// 지울부분

	std::cout << path <<"\n";
	try
	{
		std::cout << "\n\n################## START #####################\n";
		if (autoindex == true) {
			if (hasAccess(path, F_OK) && hasAccess(path, R_OK)) {
				if (is_dir(path)) {
					http_status = kOk;
					return setHtml(path);
				}
			}
		}
		http_status = kForbidden;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return "";
}
