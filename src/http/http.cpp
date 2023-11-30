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
void Http::Execute(const Conf &conf)
{
	// TODO: check URI alias

	// TODO: execute method

#if 1 // request debug
	std::cout << "######################################" << std::endl;
	std::cout << "Req method: " << request_.method() << std::endl;
	std::cout << "Req uri: " << request_.uri() << std::endl;
	std::cout << "Req version: " << request_.version() << std::endl;
	std::cout << std::endl;
	for (std::map<std::string, std::string>::iterator it = request_.headers().begin(); it != request_.headers().end(); ++it)
		std::cout << "Req " << it->first << ": " << it->second << std::endl;
	std::cout << "######################################" << std::endl;
#endif

	const std::string url = conf.GetUrl(request_.uri());
	const int location_idx = conf.GetLocationIdx(url);
	if (location_idx != -1) {
		location_t location = conf.GetLocation(location_idx);
		location.print();
	}

	// temp output
	response_.Clear();
	response_.set_status(kNotFound);
	response_.set_version(request_.version());
	response_.add_header("Content-Type", "text/plain");
	response_.set_done(true);

#if 1 // response debug
	std::cout << "######################################" << std::endl;
	std::cout << "Res version: " << response_.version() << std::endl;
	std::cout << "Res status: " << response_.status() << std::endl;
	std::cout << "Res message: " << response_.message(response_.status()) << std::endl;
	std::cout << std::endl;
	for (std::map<std::string, std::string>::iterator it = request_.headers().begin(); it != request_.headers().end(); ++it)
		std::cout << "Res " << it->first << ": " << it->second << std::endl;
	std::cout << "######################################" << std::endl;
#endif
}

void Http::Do(std::stringstream& in, std::stringstream& out, const Conf &conf)
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
			this->Execute(conf);
	}
}
