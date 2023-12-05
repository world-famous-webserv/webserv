#ifndef LIBS_CGI_CGI_HPP
# define LIBS_CGI_CGI_HPP

# include <string>       /* std::string */
# include <iostream>     /* std::cout */
# include <sstream>
# include <unistd.h>
# include <cstdio>

# include "config.hpp"
# include "http/http_request.hpp"
# include "http/http_response.hpp"

class Cgi {
	public:
		static std::string run(HttpRequest &request, const location_t &location, const std::string &script);
	private:
		~Cgi(void);
		Cgi(void);
		Cgi(const Cgi &obj);
		Cgi &operator=(const Cgi &obj);
		static std::string to_string(const int i);
};

#endif // LIBS_CGI_CGI_HPP_ */
