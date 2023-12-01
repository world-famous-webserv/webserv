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
	// process relative path
	const std::string url = conf.GetUrl(request_.uri());
	if (url.empty())
	{
		this->GenerateError(conf, kFound);
		response_.add_header("Location", "/");
		response_.set_done(true);
		return;
	}

	// get location
	const int location_idx = conf.GetLocationIdx(url);
	if (location_idx == -1)
	{
		this->GenerateError(conf, kNotFound);
		response_.set_done(true);
		return;
	}
	location_t location = conf.GetLocation(location_idx);

	// check return
	if (location.ret.code != 0)
	{
		this->GenerateError(conf, kFound);
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
				response_.add_header("Location", location.ret.text);
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
	}
	// if error
	HttpStatus status = response_.status();
	if (200 <= status && status <= 299)
		return ;
	this->GenerateError(conf, status);
	response_.set_done(true);
}

void Http::Do(std::stringstream& in, std::stringstream& out, const Conf &conf)
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
			this->Execute(conf);
	}
}
