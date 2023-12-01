#ifndef SRC_HTTP_HTTP_HPP_
# define SRC_HTTP_HTTP_HPP_
# include "http_request.hpp"
# include "http_response.hpp"
# include "conf.hpp"

class Http
{
 public:
	~Http(void);
	Http(void);

	void Do(std::stringstream& in, std::stringstream& out, const Conf &conf);
 private:
	Http(const Http& obj);
	Http& operator=(const Http& obj);

	void Execute(const Conf &conf);

	void GenerateError(const Conf &conf, HttpStatus status);

	HttpRequest request_;
	HttpResponse response_;
};

#endif /* SRC_HTTP_HTTP_HPP_ */
