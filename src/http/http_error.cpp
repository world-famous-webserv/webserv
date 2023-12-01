#include "http.hpp"

std::string default_error(HttpStatus status, std::string message)
{
	std::ostringstream oss;

	oss
		<< "<html>\r\n"
		<< "<head><title>" << status << " " << message << "</title></head>\r\n"
		<< "<body>\r\n"
		<< "<center><h1>" << status << " " << message << "</h1></center>\r\n"
		<< "<hr><center>webserv</center>\r\n"
		<< "</body>\r\n"
		<< "</html>";
	return oss.str();
}

void Http::GenerateError(const Conf& conf, HttpStatus status)
{
	response_.Clear();
	response_.set_status(status);
	response_.set_version(request_.version());
	response_.add_header("Content-Type", "text/html");
	response_.add_header("Connection", "close");

	std::map<int, std::string>::const_iterator it = conf.error_page.find(status);
	if (it != conf.error_page.end())
	{
		std::string path = conf.GetPath(it->second);
		std::ifstream page(path.c_str(), std::ios_base::in);
		if (page.is_open())
		{
			response_.body() << page.rdbuf();
			page.close();
			return;
		}
	}
	response_.body() << default_error(status, response_.message(status));
}
