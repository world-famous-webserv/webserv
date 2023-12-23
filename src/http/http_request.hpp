#ifndef SRC_HTTP_HTTP_REQUEST_HPP_
# define SRC_HTTP_HTTP_REQUEST_HPP_
# include <map>
# include <vector>
# include <sstream>
# include <cstdlib>
# include <algorithm>
# include "http_code.hpp"

class HttpRequest
{
 public:
	~HttpRequest(void);
	HttpRequest(void);
	HttpRequest& operator<<(std::stringstream& req);

	const HttpStep &step(void) const;
	const std::string &method(void) const;
	const std::string &uri(void) const;
	const std::string &version(void) const;
	const std::string header(const std::string& key) const;
	const std::map<std::string, std::string>& headers(void) const;
	std::stringstream& body(void);

	void set_step(const HttpStep &step);
	void set_method(const std::string &method);
	void set_uri(const std::string &uri);
	void set_version(const std::string &version);
	void add_header(const std::string &key, const std::string &val);

	void Clear(void);
 private:
	HttpRequest(const HttpRequest& obj);
	HttpRequest& operator=(const HttpRequest& obj);

	bool GetLine(std::stringstream& req, std::string& line);
	void ParseStartLine(const std::string& line);
	void ParseHeader(const std::string& line);
	void ParseBody(std::stringstream& req);
	void ParseChunkLen(const std::string& line);
	void ParseChunk(std::stringstream& req);

	std::string method_;
	std::string uri_;
	std::string version_;
	std::map<std::string, std::string> headers_;
	std::stringstream body_;

	// for parse
	HttpStep step_;
	std::stringstream buf_;
	std::size_t remain_;
};

#endif /* SRC_HTTP_HTTP_REQUEST_HPP_ */
