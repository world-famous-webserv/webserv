#ifndef LIBS_CGI_CGI_HPP
# define LIBS_CGI_CGI_HPP

# include <string>       /* std::string */
# include <iostream>     /* std::cout */
# include <sstream>
# include <unistd.h>
# include <cstdio>

# include "http/http_request.hpp"
# include "http/http_response.hpp"
# include "config.hpp"

class Cgi {
	public:
		~Cgi(void);
		explicit Cgi(HttpRequest &request, const location_t &location);
		Cgi(const Cgi &obj);
		Cgi &operator=(const Cgi &obj);
		std::string run(std::string script);
	private:
		Cgi(void);
		std::map<std::string, std::string>	env_;
		std::string							request_body_;

		std::string to_string(const int i);
};

#endif // LIBS_CGI_CGI_HPP_ */
