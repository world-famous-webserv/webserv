#include "http.hpp"

/* ************************************************************************** */
// Orthodox Canonical Form
/* ************************************************************************** */

Http::~Http(void)
{
}

Http::Http(const Conf &conf):
	conf_(conf)
{
}

/* ************************************************************************** */
// Main
/* ************************************************************************** */

#include <iostream>
void Http::Execute(const Conf &conf)
{
	// process relative path
	const std::string url = conf_.GetUrl(request_.uri());
	if (url.empty())
	{
		this->GenerateError(kFound);
		response_.add_header("Location", "/");
		response_.set_done(true);
		return;
	}
	std::cout << "url: " << url << "\n";

	// get location
	const int location_idx = conf_.GetLocationIdx(url);
	if (location_idx == -1)
	{
		this->GenerateError(kNotFound);
		response_.set_done(true);
		return;
	}
	location_t location = conf_.GetLocation(location_idx);

	// check return
	if (location.ret.code != 0)
	{
		this->GenerateError(kFound);
		switch(location.ret.code)
		{
			case kMovedPermanently:
			case kFound:
			case kSeeOther:
			case kTemporaryRedirect:
			case kPermanentRedirect:
				response_.add_header("Location", location.ret.url);
				break;
			default:
				response_.body().str(location.ret.text);
				break;
		}
		response_.set_done(true);
		return;
	}

	// check limit_except


	// execute method
	{
		// temp get
		std::string path(conf.GetPath(url));
		std::cout << "path: " << path << std::endl;
		std::fstream get(path.c_str());
		if (get.is_open())
		{
			std::cout << "read: " << path << std::endl;
			response_.set_status(kOk);
			response_.set_version(request_.version());
			response_.add_header("Content-Type", "text/html");
			response_.add_header("Connection", "keep-alive");
			response_.body() << get.rdbuf();
			response_.set_done(true);
			get.close();
			return;
		}
		response_.set_status(kNotFound);
	}
	// if error
	if (200 <= status && status <= 299)
		return ;
	this->GenerateError(status);
	response_.set_done(true);
}

void Http::Do(std::stringstream& in, std::stringstream& out)
{
	if (response_.done())
	{
		response_ >> out;
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
    return kOk;
}
