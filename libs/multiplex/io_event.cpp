#include "io_event.hpp"

/* ************************************************************************** */
// Orthodox Canonical Form
/* ************************************************************************** */

IOEvent::~IOEvent(void)
{
	this->Close();
}

IOEvent::IOEvent(void)
	: identifier_(-1)
{
}

IOEvent::IOEvent(const IOEvent& obj)
{
	*this = obj;
}

IOEvent& IOEvent::operator=(const IOEvent& obj)
{
	if (this == &obj)
		return *this;
	identifier_ = obj.identifier_;
	return *this;
}

/* ************************************************************************** */
// Main
/* ************************************************************************** */

int IOEvent::identifier(void) const
{
	return identifier_;
}

void IOEvent::Close(void)
{
	if (identifier_ > 0)
		close(identifier_);
	identifier_ = -1;
}

void IOEvent::Broken(int errcode)
{
	errno = errcode;
	if (errno)
		perror("Broken");
	errno = 0;
	this->Close();
}
