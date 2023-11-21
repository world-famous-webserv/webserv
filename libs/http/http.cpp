#include "http.hpp"

/* ************************************************************************** */
// Orthodox Canonical Form
/* ************************************************************************** */

Http::~Http(void)
{
}

Http::Http(void)
{
}

/* ************************************************************************** */
// Main
/* ************************************************************************** */

#include <iostream>
void Http::Execute(void)
{
	// TODO: check URI alias

	// TODO: execute method

#if 1 // request debug
	std::cout << "######################################" << std::endl;
	std::cout << "Req method: " << request_.method() << std::endl;
	std::cout << "Req uri: " << request_.uri() << std::endl;
	std::cout << "Req version: " << request_.version() << std::endl;
	std::cout << std::endl;
	std::cout << "Req Host: " << request_.header("Host") << std::endl;
	std::cout << "Req Accept: " << request_.header("Accept") << std::endl;
	std::cout << "Req User-Agent: " << request_.header("User-Agent") << std::endl;
	std::cout << "######################################" << std::endl;
#endif

	// temp output
	response_.Clear();
	response_.set_status(kNotFound);
	response_.set_version(request_.version());
	response_.add_header("Content-Type", "text/plain");
	response_.add_header("Content-Length", "0");
	response_.set_done(true);

#if 1 // response debug
	std::cout << "######################################" << std::endl;
	std::cout << "Res version: " << response_.version() << std::endl;
	std::cout << "Res status: " << response_.status() << std::endl;
	std::cout << "Res message: " << response_.message(response_.status()) << std::endl;
	std::cout << std::endl;
	std::cout << "Res Content-Type: " << response_.header("Content-Type") << std::endl;
	std::cout << "Res Content-Length: " << response_.header("Content-Length") << std::endl;
	std::cout << "######################################" << std::endl;
#endif
}

void Http::Do(std::stringstream& in, std::stringstream& out)
{
	if (response_.done())
	{
		response_ >> out;
std::cout << "Done!" << std::endl;
		request_.Clear();
		response_.Clear();
		return ;
	}
	if (!request_.done())
	{
		request_ << in;
		if (request_.done())
			this->Execute();
	}
}
