#include "http_request.hpp"
#include <iostream>

/* ************************************************************************** */
// Orthodox Canonical Form
/* ************************************************************************** */

HttpRequest::~HttpRequest(void)
{
}

HttpRequest::HttpRequest(void)
{
	this->Clear();
}

/* ************************************************************************** */
// get / set
/* ************************************************************************** */

void HttpRequest::set_step(const HttpStep &step)
{
	step_ = step;
}

const HttpStep &HttpRequest::step(void) const
{
	return step_;
}

const std::string &HttpRequest::method(void) const
{
	return method_;
}

void HttpRequest::set_method(const std::string &method)
{
	method_ = method;
}

const std::string &HttpRequest::uri(void) const
{
	return uri_;
}

void HttpRequest::set_uri(const std::string &uri)
{
	uri_ = uri;
}

const std::string &HttpRequest::version(void) const
{
	return version_;
}

void HttpRequest::set_version(const std::string &version)
{
	version_ = version;
}

const std::string HttpRequest::header(const std::string& key) const
{
	std::string lower(key);
	std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
	std::map<std::string, std::string>::const_iterator it = headers_.find(lower);

	return it != headers_.end()? it->second : "";
}

const std::map<std::string, std::string>& HttpRequest::headers(void) const
{
	return headers_;
}

void HttpRequest::add_header(const std::string &key, const std::string &val)
{
	std::string lower(key);
	std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
	headers_[lower] = val;
}

std::stringstream& HttpRequest::body(void)
{
	return body_;
}

/* ************************************************************************** */
// Parse
/* ************************************************************************** */

bool HttpRequest::GetLine(std::stringstream& req, std::string& line)
{
	req.clear();
	while (req.good())
	{
		char ch = req.get();
		if (!req.good())
			return false;
		if (ch != '\n')
		{
			buf_.put(ch);
			continue;
		}
		std::getline(buf_, line);
		if (line[line.length() - 1] == '\r')
			line.erase(line.length() - 1);
		buf_.str();
		buf_.clear();
		return true;
	}
	return false;
}

void HttpRequest::ParseStartLine(const std::string& line)
{
	std::istringstream iss(line);

	std::string method;
	std::getline(iss, method, ' ');
	this->set_method(method);

	std::string uri;
	std::getline(iss, uri, ' ');
	this->set_uri(uri);

	std::string version;
	std::getline(iss, version);
	this->set_version(version);

	step_ = kParseHeader;
}

void HttpRequest::ParseHeader(const std::string& line)
{
	if (line.empty())
	{
		std::string len = this->header("Content-Length");
		if (!len.empty())
		{
			std::stringstream ss(len);
			ss >> remain_;
			step_ = kParseBodyStart;
			return;
		}
		std::string chunk = this->header("Transfer-Encoding");
		std::transform(chunk.begin(), chunk.end(), chunk.begin(), ::tolower);
		if (!chunk.empty() && chunk == "chunked")
		{
			step_ = kParseChunkStart;
			return;
		}
		remain_ = 0;
		step_ = kParseBodyStart;
		return;
	}
	std::istringstream iss(line);
	std::string key, val;
	std::getline(iss, key, ':');
	std::getline(iss, val);
	this->add_header(key, val);
	std::cout << "Header: " << key << " = " << val << std::endl;
}

void HttpRequest::ParseBody(std::stringstream& req)
{
	char buf[1024];

	req.clear();
	while (req.good() && remain_ > 0)
	{
		std::size_t len = std::min(remain_, sizeof(buf));
		if (req.read(buf, len))
			body_.write(buf, req.gcount());
		remain_ -= req.gcount();
	}
	if (remain_ == 0)
		step_ = kParseDone;
}

void HttpRequest::ParseChunkLen(const std::string& line)
{
	std::stringstream ss;
	ss << std::hex << line;
	ss >> remain_;
	if (remain_ == 0)
		step_ = kParseDone;
	else
		step_ = kParseChunkBody;
}

void HttpRequest::ParseChunk(std::stringstream& req)
{
	char buf[1024];

	req.clear();
	while (req.good() && remain_ > 0)
	{
		std::size_t len = std::min(remain_, sizeof(buf));
		if (req.read(buf, len))
			body_.write(buf, req.gcount());
		remain_ -= req.gcount();
	}
	if (remain_ == 0)
		step_ = kParseChunkLen;
}


/* ************************************************************************** */
// Main
/* ************************************************************************** */

void HttpRequest::Clear(void)
{
	method_.clear();
	uri_.clear();
	version_.clear();
	headers_.clear();
	body_.clear();

	// for parse
	step_ = kParseStart;
	buf_.str();
	buf_.clear();
	remain_ = 0;
}

HttpRequest& HttpRequest::operator<<(std::stringstream& req)
{
	req.clear();
	std::string line;
	if (step_ == kParseStart && this->GetLine(req, line) == true)
		this->ParseStartLine(line);
	if (step_ == kParseHeader && this->GetLine(req, line) == true)
		this->ParseHeader(line);
	if (step_ == kParseChunkLen && this->GetLine(req, line) == true)
		this->ParseChunkLen(line);
	else if (step_ == kParseBody)
		this->ParseBody(req);
	else if (step_ == kParseChunkBody)
		this->ParseChunk(req);
	return *this;
}
