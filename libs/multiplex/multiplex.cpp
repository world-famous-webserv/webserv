#ifdef __LINUX__
# include <sys/epoll.h>
#else
# include <sys/event.h>
#endif
#include <cerrno>
#include "multiplex.hpp"

/* ************************************************************************** */
// Orthodox Canonical Form
/* ************************************************************************** */

Multiplex::~Multiplex(void)
{
	for (IOMap::iterator it = ios_.begin(); it != ios_.end(); ++it)
		delete it->second;
	if (handler_ > 0)
		close(handler_);
	handler_ = -1;
}

Multiplex::Multiplex(void)
{
#ifdef __LINUX__
	handler_ = epoll_create1(0);
#else
	handler_ = kqueue();
#endif
	if (handler_ == -1)
		throw std::runtime_error(strerror(errno));
}

Multiplex::Multiplex(const Multiplex& obj)
{
	*this = obj;
}

Multiplex& Multiplex::operator=(const Multiplex& obj)
{
	if (this == &obj)
		return (*this);
	handler_ = obj.handler_;
	return (*this);
}

/* ************************************************************************** */
// Main
/* ************************************************************************** */

Multiplex& Multiplex::GetInstance(void)
{
	static Multiplex instance;
	return instance;
}

void Multiplex::AddItem(IOEvent* event)
{
	IOMap::iterator it = ios_.find(event->identifier());
	if (it != ios_.end())
	{
		delete it->second;
		ios_.erase(it);
	}
#ifdef __LINUX__
	struct epoll_event changes;
	changes.events = EPOLLIN | EPOLLOUT;
	changes.data.ptr = event;
	if (epoll_ctl(handler_, EPOLL_CTL_ADD, event->identifier(), &changes) == -1)
		throw std::runtime_error(strerror(errno));
#else
	struct kevent changes[2];
	EV_SET(&changes[0], event->identifier(), EVFILT_READ, EV_ADD, 0, 0, event);
	EV_SET(&changes[1], event->identifier(), EVFILT_WRITE, EV_ADD, 0, 0, event);
    if (kevent(handler_, changes, 2, NULL, 0, NULL) == -1)
		throw std::runtime_error(strerror(errno));
#endif
	ios_[event->identifier()] = event;
}

void Multiplex::Loop(void)
{
	static const int	kMaxEvent = 10;

	while (true)
	{
#ifdef __LINUX__
		struct epoll_event eventlist[kMaxEvent];
		int nevents = epoll_wait(handler_, eventlist, kMaxEvent, -1);
		if (nevents == - 1)
			throw std::runtime_error(strerror(errno));
		for (int i = 0; i < nevents; i++)
		{
			IOEvent *event = static_cast<IOEvent*>(eventlist[i].data.ptr);
			if (eventlist[i].events & (EPOLLERR | EPOLLHUP))
				event->Broken(0);
			else if (eventlist[i].events & EPOLLRDHUP)
				event->Close();
			else if (eventlist[i].events & EPOLLIN)
				event->Read();
			else if (eventlist[i].events & EPOLLOUT)
				event->Write();
		}
#else
		struct kevent eventlist[kMaxEvent];
		int nevents = kevent(handler_, NULL, 0, eventlist, kMaxEvent, NULL);
		if (nevents == - 1)
			throw std::runtime_error(strerror(errno));
		for (int i = 0; i < nevents; i++)
		{
			IOEvent *event = static_cast<IOEvent*>(eventlist[i].udata);
			if (eventlist[i].flags & EV_ERROR)
				event->Broken(eventlist[i].data);
			else if (eventlist[i].filter & EVFILT_READ)
				event->Read();
			else if (eventlist[i].filter & EVFILT_WRITE)
				event->Write();
		}
#endif
	}
}
