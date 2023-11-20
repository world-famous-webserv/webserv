#include "http.hpp"

/* ************************************************************************** */
// Orthodox Canonical Form
/* ************************************************************************** */

Http::~Http(void)
{
}

Http::Http(void)
	: state_(kRequestParse)
{
}

Http::Http(const Http& obj)
{
	*this = obj;
}

Http& Http::operator=(const Http& obj)
{
	if (this == &obj)
		return *this;
	return *this;
}

/* ************************************************************************** */
// read/write
/* ************************************************************************** */

# include <iostream>
Http& Http::operator<<(int fd)
{
	// recv
	char buf[1024] = {0};
	ssize_t nbytes = recv(fd, buf, sizeof(buf), 0);
	if (nbytes < 0)
		throw std::runtime_error("Failed to recv");
	// put
	if (nbytes > 0) {
		request_.clear();
		request_.write(buf, nbytes);
	}
	return *this;
}

Http& Http::operator>>(int fd)
{
	// get
	char buf[1024] = {0};
	response_.clear();
	response_.read(buf, sizeof(buf));
	if (response_.gcount() <= 0)
		return *this;
	// send
	ssize_t nbytes = send(fd, buf, response_.gcount(), 0);
	if (nbytes < 0)
		throw std::runtime_error("Failed to send");
	if (nbytes == 0)
		return *this;
	response_.seekg(nbytes, std::ios::cur);
	return *this;
}

/* ************************************************************************** */
// Main
/* ************************************************************************** */

std::string Http::GetHeader(const std::string& key) const
{
	std::map<std::string, std::string>::const_iterator it = headers_.find(key);
	return it != headers_.end()? it->second : "";
}

void Http::Parse(void)
{
	request_.clear();
	std::streampos curr = request_.tellg();

	// start line
	std::getline(request_, method_, ' ');
	std::getline(request_, uri_, ' ');
	std::getline(request_, version_, '\n');
	if (version_[version_.length() - 1] == '\r')
		version_.erase(version_.length() - 1);

	// header
	std::string line;
	while (std::getline(request_, line)) {
		if (request_.eof())
			break ;
		if (line.empty() || line.compare("\r") == 0)
			break ;
		std::istringstream header(line);
		std::string key, val;
		std::getline(header, key, ':');
		std::getline(header, val, '\r');
		headers_[key] = val;
	}

	// body
	int len = std::atoi(this->GetHeader("Content-Length").c_str());
	if (len > 0) {
		char *buffer = new char[len];
		request_.read(buffer, len);
		body_.assign(buffer, len);
		delete[] buffer;
	}
	if (!request_.good()) {
		request_.clear();
		request_.seekg(curr);
	} else 
		state_ = kRequestRun;
}

void Http::Run(void)
{
	// TODO: check URI alias

	// temp check
	std::cout << "Method: " << method_ << std::endl;
	std::cout << "Uri: " << uri_ << std::endl;
	std::cout << "version: " << version_ << std::endl;
	for (std::map<std::string, std::string>::const_iterator it = headers_.begin(); it != headers_.end(); ++it)
		std::cout << it->first << ": " << it->second << std::endl;
	std::cout << "body: " << body_ << std::endl;
	std::cout << "########################" << std::endl;

	state_ = kResponseWait;
	// temp output
	response_.clear();
	response_ << "HTTP/1.1 404 Not Found\r\n";
	response_ << "Content-Type: text/plain\r\n";
	response_ << "Content-Length: 0\r\n";
	response_ << "\r\n";
	std::cout << response_.str() << std::endl;
	state_ = kResponseEnd;
}

void Http::Clear(void)
{
	method_.clear();
	uri_.clear();
	version_.clear();
	body_.clear();
	state_ = kRequestParse;
}

void Http::Do(void)
{
	switch (state_) {
		case kRequestParse: this->Parse(); break;
		case kRequestRun: this->Run(); break;
		case kResponseEnd: this->Clear(); break;
		default:
		  break;
	}
}
