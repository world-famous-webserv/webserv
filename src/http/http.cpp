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
	response_.set_status(kOk);
	response_.set_version(request_.version());
	response_.add_header("Connection", "keep-alive");
	response_.add_header("Content-Type", "text/html");

	std::cout << "Uri = " << request_.uri() << std::endl;
	std::cout << "Url = " << conf_.GetUrl(request_.uri()) << std::endl;
	std::cout << "Path = " << conf_.GetPath(conf_.GetUrl(request_.uri())) << std::endl;
	request_.set_step(kExecuteDone);

	std::cout << "EXECUTE!" << std::endl;
	// process relative path
	const std::string url = conf_.GetUrl(request_.uri());
	if (url.empty())
	{
		response_.add_header("Location", "/");
		return response_.GenerateError(kFound, conf_.error_page);
	}

	// get location
	const int location_idx = conf_.GetLocationIdx(url);
	if (location_idx == -1)
		return response_.GenerateError(kNotFound, conf_.error_page);
	location_t location = conf_.GetLocation(location_idx);

	// client_max_body_size
	if (request_.body().str().length() > static_cast<size_t>(location.client_max_body_size))
	{
std::cerr << "body len : " << request_.body().tellp() << std::endl;
std::cerr << "max body : " << location.client_max_body_size << std::endl;
		return response_.GenerateError(kPayloadTooLarge, location.error_page);
	}

	// check return
	if (location.ret.code != 0)
	{
		response_.GenerateError(static_cast<HttpStatus>(location.ret.code), location.error_page);
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
		return;
	}

	// check limit_except
	const std::vector<std::string> &methods = location.limit_except.methods;
	if (std::find(methods.begin(), methods.end(), request_.method()) == methods.end())
	{
		const std::vector<std::string> &allows = location.limit_except.allows;
		const std::vector<std::string> &denys = location.limit_except.denys;

		if (std::find(allows.begin(), allows.end(), "all") == allows.end()
			&& std::find(denys.begin(), denys.end(), "all") != denys.end())
			return response_.GenerateError(kMethodNotAllowed, location.error_page);
	}
	
	// execute method
	if (request_.method().compare("GET") == 0)
		return this->Get(location, url);
	else if (request_.method().compare("PUT") == 0)
		return this->Put(location, url);
	else if (request_.method().compare("POST") == 0)
		return this->Post(location, url);
	else if (request_.method().compare("DELETE") == 0)
		return this->Delete(location, url);
	response_.GenerateError(kMethodNotAllowed, location.error_page);
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
	request_ << in;
	if (request_.step() == kParseFail)
		response_.set_status(kBadRequest);
	else if (request_.step() == kParseDone)
		this->Execute();
}
