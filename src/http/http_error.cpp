#include "http.hpp"

static const std::string default_error(const HttpStatus &status, const std::string &message)
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

void HttpResponse::GenerateError(HttpStatus status, const std::map<int, std::string>& error_page)
{
	this->set_done(true);
	this->set_status(status);
	this->add_header("Connection", "close");
	this->add_header("Content-Type", "text/html");

	// error_page not found
	std::map<int, std::string>::const_iterator it = error_page.find(status);
	if (it == error_page.end())
	{
		this->body() << default_error(status, this->message(status));
		return;
	}

	// error_page found!
	std::ifstream page(it->second, std::ios_base::in);
	if (page.is_open())
	{
		this->body() << page.rdbuf();
		page.close();
		return;
	}

	// file cannot open
	this->body() << default_error(status, this->message(status));
}
