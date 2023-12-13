#ifndef SRC_HTTP_CGI_HPP_
# define SRC_HTTP_CGI_HPP_
# include <fcntl.h>
# include <sys/socket.h>
# include <sys/wait.h>
# include <csignal>
# include "io_event.hpp"
# include "conf.hpp"
# include "http_request.hpp"
# include "http_response.hpp"

class Cgi : public IOEvent
{
 public:
	Cgi(const location_t& location, HttpRequest& req, HttpResponse& res);
	virtual ~Cgi(void);

	void Open(void);
	void Read(void);
	void Write(void);
	void Update(void);

	static std::string GetCgi(const location_t& location, const std::string path);
 private:
	Cgi(void);
	Cgi(const Cgi &obj);
	Cgi& operator=(const Cgi &obj);

	void Child(void);

	pid_t pid_;
	const location_t& location_;
	HttpRequest& request_;
	HttpResponse& response_;

	// for cgi parse
	const std::string header(const std::string& key) const;
	void add_header(const std::string &key, const std::string &val);

	bool GetLine(std::stringstream& in, std::string& line);
	void ParseHeader(const std::string& line);
	void ParseBody(void);
	void ParseDone(void);

	HttpStep step_;
	std::stringstream buf_;
	std::map<std::string, std::string> headers_;
	std::stringstream analyzer_;
};

# endif /* SRC_HTTP_CGI_HPP_ */
