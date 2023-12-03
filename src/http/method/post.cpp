#include "../http.hpp"

#include <iostream>
HttpStatus Http::Post()
{
	std::cout << request_.method() << std::endl;
	std::cout << request_.uri() << std::endl;
	std::cout << request_.version() << std::endl;

	const std::map<std::string, std::string> &headers = request_.headers();
	for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it)
		std::cout << it->first << ": " << it->second << std::endl;
	
	std::cout << "body:" << std::endl;
	std::cout << request_.body().str() << std::endl;
	response_.set_done(true);
	return kOk;
}
