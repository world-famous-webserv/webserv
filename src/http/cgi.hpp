#ifndef SRC_HTTP_CGI_HPP_
# define SRC_HTTP_CGI_HPP_
# include <sys/wait.h>
# include "io_event.hpp"
# include "conf.hpp"
# include "http_request.hpp"
# include "http_response.hpp"

class Cgi : public IOEvent
{
 public:
	Cgi(const std::string &cgi_pass, HttpRequest& req, HttpResponse& res);
	virtual ~Cgi(void);

	void Open(void);
	void Read(void);
	void Write(void);
	void Update(void);

	static std::string GetCgiPass(const location_t &location, const std::string &path);
 private:
	Cgi(void);
	Cgi(const Cgi &obj);
	Cgi& operator=(const Cgi &obj);

	void Child(void);

	pid_t pid_;

	std::string cgipass_;
	std::string path_;

	HttpRequest& request_;
	HttpResponse& response_;
};

# endif /* SRC_HTTP_CGI_HPP_ */
