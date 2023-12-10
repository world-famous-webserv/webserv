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
	std::cout << "Uri = " << request_.uri() << std::endl;
	std::cout << "Url = " << conf_.GetUrl(request_.uri()) << std::endl;
	std::cout << "Path = " << conf_.GetPath(conf_.GetUrl(request_.uri())) << std::endl;
	request_.set_step(kExecuteDone);
	if (request_.body().str().length() > static_cast<size_t>(conf_.client_max_body_size))
	{
		this->GenerateError(kPayloadTooLarge);
		return response_.set_done(true);
	}

	std::cout << "EXECUTE!" << std::endl;
	// process relative path
	const std::string url = conf_.GetUrl(request_.uri());
	if (url.empty())
	{
		this->GenerateError(kFound);
		response_.add_header("Location", "/");
		return response_.set_done(true);
	}

	// get location
	const int location_idx = conf_.GetLocationIdx(url);
	if (location_idx == -1)
	{
		this->GenerateError(kNotFound);
		return response_.set_done(true);
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
		return response_.set_done(true);
	}

	// check limit_except
	const std::vector<std::string> &methods = location.limit_except.methods;
	if (std::find(methods.begin(), methods.end(), request_.method()) == methods.end())
	{
		const std::vector<std::string> &allows = location.allows;
		const std::vector<std::string> &denys = location.denys;

		if (std::find(allows.begin(), allows.end(), "all") == allows.end() && \
			std::find(denys.begin(), denys.end(), "all") != denys.end())
		{
			this->GenerateError(kMethodNotAllowed);
			response_.set_done(true);
			return;
		}
	}

	// execute method
	HttpStatus status = kOk;
	if (request_.method().compare("GET") == 0)
		status = this->Get(location, url);
	else if (request_.method().compare("DELETE") == 0)
		status = this->Delete(url);
	else if (request_.method().compare("POST") == 0)
		status = this->Post(location, url);
	else
		status = kMethodNotAllowed;
	// if error
	if (200 <= status && status <= 299) {
		response_.set_status(status);
		return;
	}
	this->GenerateError(status);
	response_.set_done(true);
}


void Http::Do(std::stringstream& in, std::stringstream& out)
{
	if (request_.step() == kParseStart)
		std::cout << "kParseStart" << std::endl;
	if (request_.step() == kParseHeader)
		std::cout << "kParseHeader" << std::endl;
	if (request_.step() == kParseBody)
		std::cout << "kParseBody" << std::endl;
	if (request_.step() == kParseChunkLen)
		std::cout << "kParseChunkLen" << std::endl;
	if (request_.step() == kParseChunkBody)
		std::cout << "kParseChunkBody" << std::endl;
	if (request_.step() == kParseDone)
		std::cout << "kParseDone" << std::endl;
	if (request_.step() == kParseFail)
		std::cout << "kParseFail" << std::endl;


	if (response_.done())
	{
		response_ >> out;
		request_.Clear();
		response_.Clear();
		return ;
	}
	request_ << in;
	if (request_.step() == kParseDone)
	{
		this->Execute();
std::cout << "### " << request_.body().rdbuf() << std::endl;
	}
	if (request_.step() == kParseFail)
	{
		this->GenerateError(kBadRequest);
		response_.set_done(true);
	}
}
