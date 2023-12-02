#ifndef SRC_HTTP_METHOD_INDEX_HPP
# define SRC_HTTP_METHOD_INDEX_HPP

# include <dirent.h>
# include <fstream>
# include <unistd.h>
# include <sys/stat.h>
# include "../http.hpp"

bool	index(const HttpRequest& req, HttpResponse& res, const location_t &location, std::string path);
void	autoindex(const HttpRequest& req, HttpResponse& res, std::string & path);

#endif /* SRC_HTTP_METHOD_INDEX_HPP */
