#include "http_request.hpp"

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

bool HttpRequest::done(void) const
{
	return done_;
}

std::string HttpRequest::method(void) const
{
	return method_;
}

void HttpRequest::set_method(const std::string method)
{
	method_ = method;
}

std::string HttpRequest::uri(void) const
{
	return uri_;
}

void HttpRequest::set_uri(const std::string uri)
{
	uri_ = uri;
}

std::string HttpRequest::version(void) const
{
	return version_;
}

void HttpRequest::set_version(const std::string version)
{
	version_ = version;
}

std::string HttpRequest::header(const std::string& key) const
{
	std::string lower(key);
	std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
	std::map<std::string, std::string>::const_iterator it = headers_.find(lower);
	return it != headers_.end()? it->second : "";
}

std::map<std::string, std::string>& HttpRequest::headers(void)
{
	return headers_;
}

void HttpRequest::add_header(const std::string key, const std::string val)
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
// Main
/* ************************************************************************** */

void HttpRequest::Clear(void)
{
	done_ = false;
	method_.clear();
	uri_.clear();
	version_.clear();
	headers_.clear();
	body_.clear();
}

HttpRequest& HttpRequest::operator<<(std::stringstream& req)
{
	this->Clear();
	req.clear();
	std::streampos curr = req.tellg();

	// start line
	std::getline(req, method_, ' ');
	std::getline(req, uri_, ' ');
	std::getline(req, version_, '\n');
	if (version_[version_.length() - 1] == '\r')
		version_.erase(version_.length() - 1);

	// header
	std::string line;
	while (std::getline(req, line)) {
		if (req.eof())
			break ;
		if (line.empty() || line.compare("\r") == 0)
			break ;
		std::istringstream header(line);
		std::string key, val;
		std::getline(header, key, ':');
		std::getline(header, val, '\r');
		this->add_header(key, val);
	}

	// body
	int len = std::atoi(header("Content-Length").c_str());
	if (len > 0) {
		char *buffer = new char[len];
		req.read(buffer, len);
		body_.write(buffer, len);
		delete[] buffer;
	}
	if (!req.good()) {
		req.clear();
		req.seekg(curr);
		return *this;
	}
	done_ = true;
	return *this;
}
