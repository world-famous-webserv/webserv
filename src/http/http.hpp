#ifndef SRC_HTTP_HTTP_HPP_
# define SRC_HTTP_HTTP_HPP_
# include "http_request.hpp"
# include "http_response.hpp"
# include "conf.hpp"

# include "method/index.hpp"

class Http
{
 public:
	~Http(void);
	Http(const Conf &conf);

	void Do(std::stringstream& in, std::stringstream& out);
 private:
	Http(const Http& obj);
	Http& operator=(const Http& obj);

	void Execute();

	void GenerateError(HttpStatus status);
	HttpStatus Get(const location_t& location, const std::string url);
	HttpStatus Post(const location_t& location, const std::string url);
	HttpStatus Delete(const std::string url);

	HttpRequest request_;
	HttpResponse response_;
	const Conf &conf_;
};

#endif /* SRC_HTTP_HTTP_HPP_ */
