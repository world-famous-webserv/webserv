#include "server.hpp"

/* ************************************************************************** */
// Orthodox Canonical Form
/* ************************************************************************** */

Client::~Client(void)
{
}

Client::Client(const int fd, const Conf &conf):
	conf_(conf)
{
	identifier_ = fd;
	this->Open();
}

/* ************************************************************************** */
// Utility
/* ************************************************************************** */

/* ************************************************************************** */
// Main
/* ************************************************************************** */

void Client::Open(void)
{
	std::cout << "Client::Open" << std::endl;
}

void Client::Read(void)
{
	if (identifier_ == -1)
		return ;
	// recv
	char buf[1024] = {0};
	ssize_t nbytes = recv(identifier_, buf, sizeof(buf), 0);
	if (nbytes < 0) return this->Broken(errno);
	if (nbytes == 0) return this->Close();
	// put
	in_.clear();
	in_.write(buf, nbytes);
}

void Client::Write(void)
{
	if (identifier_ == -1)
		return ;
	// get
	char buf[1024] = {0};
	out_.clear();
	out_.read(buf, sizeof(buf));
	if (out_.gcount() <= 0) return;
	// send
	ssize_t nbytes = send(identifier_, buf, out_.gcount(), 0);
	if (nbytes < 0) return this->Broken(errno);
	if (nbytes == 0) return this->Close();
	out_.seekg(nbytes, std::ios::cur);
}

void Client::Update(void)
{
	if (identifier_ == -1)
		return ;
	http_.Do(in_, out_, conf_);
}
