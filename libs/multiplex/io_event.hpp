#pragma once
#ifndef LIBS_MULTIPLEX_IO_EVENT_HPP_
# define LIBS_MULTIPLEX_IO_EVENT_HPP_
# include <unistd.h>
# include <sys/socket.h>
# include <cstdio>
# include <cerrno>
# include <cstring>
# include <iostream>
# include "multiplex.hpp"

class IOEvent
{
 public:
	IOEvent(void);
	IOEvent(const IOEvent &obj);
	IOEvent& operator=(const IOEvent &obj);
	virtual ~IOEvent(void);

	int identifier(void) const;
	void Close(void);
	void Broken(int errcode);
	virtual void Open(void) = 0;
	virtual void Read(void) = 0;
	virtual void Write(void) = 0;
 protected:
	int identifier_;
};

#endif /* LIBS_MULTIPLEX_IO_EVENT_HPP_ */
