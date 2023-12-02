#ifndef SRC_HTTP_METHOD_INDEX_HPP
# define SRC_HTTP_METHOD_INDEX_HPP

# include <string>
# include <dirent.h>
# include <fstream>
# include <sstream>
# include <iostream>
# include "../http_code.hpp"
# include <unistd.h>
#include <sys/stat.h>

bool		hasAccess(std::string & fileName, int mode);
bool		index();
std::string	autoindex(std::string path);
std::string	setHtml(std::string & path);
std::string	setHead(std::string & path);
std::string	setEnd();
std::string	setAtag(std::string pathname, std::string filename);
std::string setAtagLine(std::string filename, char * time,  off_t filesize);

#endif /* SRC_HTTP_METHOD_INDEX_HPP */
