#ifndef LIBS_HTTP_HTTP_HPP_
# define LIBS_HTTP_HTTP_HPP_
# include <sstream>
# include <sys/types.h>
# include <sys/socket.h>
# include <vector>
# include <map>
# include <cstdlib>
# include "http_code.hpp"

class Http
{
 public:
	virtual ~Http(void);
	Http(void);
	Http(const Http& obj);
	Http& operator=(const Http& obj);

	// for request
	Http& operator<<(int fd);
	
	// for response
	Http& operator>>(int fd);

	void Do(void);
	std::string GetHeader(const std::string& key) const;
 protected:
 private:
	void Clear(void);
	void Parse(void);
	void Run(void);

	std::stringstream request_;
	std::stringstream response_;
	
	enum HttpState state_;
	std::string method_;
	std::string uri_;
	std::string version_;
	std::map<std::string, std::string> headers_;
	std::string body_;
};

#endif /* LIBS_HTTP_HTTP_HPP_ */
