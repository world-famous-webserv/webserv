#ifndef SRC_HTTP_HTTP_RESPONSE_HPP_
# define SRC_HTTP_HTTP_RESPONSE_HPP_
# include <map>
# include <sstream>
# include <algorithm>
# include "http_code.hpp"
# include "cookie.hpp"

class HttpResponse
{
 public:
	~HttpResponse(void);
	HttpResponse(void);
	HttpResponse& operator>>(std::stringstream& res);

	const bool &done(void) const;
	const std::string &version(void) const;
	const enum HttpStatus &status(void) const;
	const std::string message(const enum HttpStatus &status) const;
	const std::string header(const std::string& key) const;
	const std::map<std::string, std::string>& headers(void) const;
	std::stringstream& body(void);

	void set_done(const bool &done);
	void set_status(const enum HttpStatus &status);
	void set_version(const std::string &version);
	void add_header(const std::string &key, const std::string &val);

	void Clear(void);
 private:
	HttpResponse(const HttpResponse& obj);
	HttpResponse& operator=(const HttpResponse& obj);

	bool done_;
	std::string version_;
	enum HttpStatus status_;
	std::map<std::string, std::string> headers_;
	std::stringstream body_;
};

#endif /* SRC_HTTP_HTTP_RESPONSE_HPP_ */
