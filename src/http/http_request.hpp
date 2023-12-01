#ifndef SRC_HTTP_HTTP_REQUEST_HPP_
# define SRC_HTTP_HTTP_REQUEST_HPP_
# include <map>
# include <sstream>
# include <cstdlib>
# include <algorithm>

class HttpRequest
{
 public:
	~HttpRequest(void);
	HttpRequest(void);
	HttpRequest& operator<<(std::stringstream& req);

	bool done(void) const;
	std::string method(void) const;
	std::string uri(void) const;
	std::string version(void) const;
	std::string header(const std::string& key) const;
	std::map<std::string, std::string>& headers(void);
	std::stringstream& body(void);

	void set_method(const std::string method);
	void set_uri(const std::string uri);
	void set_version(const std::string version);
	void add_header(const std::string key, const std::string val);
	void add_body(const char *buf, ssize_t len);

	void Clear(void);
 private:
	HttpRequest(const HttpRequest& obj);
	HttpRequest& operator=(const HttpRequest& obj);

	bool done_;
	std::string method_;
	std::string uri_;
	std::string version_;
	std::map<std::string, std::string> headers_;
	std::stringstream body_;
};

#endif /* SRC_HTTP_HTTP_REQUEST_HPP_ */
