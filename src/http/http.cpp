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
void Http::Execute()
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
	HttpStatus status = kOk;
	if (request_.method().compare("GET") == 0)
		status = this->Get(location, url);
	else if (request_.method().compare("DELETE") == 0)
		status = this->Delete(url);
	else if (request_.method().compare("POST") == 0)
		status = this->Post();
	else
		status = kMethodNotAllowed;
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
