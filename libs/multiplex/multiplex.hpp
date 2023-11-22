#ifndef LIBS_MULTIPLEX_MULTIPLEX_HPP_
# define LIBS_MULTIPLEX_MULTIPLEX_HPP_
# include <string.h>
# include <map>
# include <stdexcept>
# include "io_event.hpp"

class IOEvent;

class Multiplex
{
 public:
	static Multiplex& GetInstance(void);
	void AddItem(IOEvent* item);
	void Loop(void);
 private:
	typedef std::map<int, IOEvent*> IOMap;

	~Multiplex();
	Multiplex();
	Multiplex(const Multiplex &obj);
	Multiplex& operator=(const Multiplex &obj);

	void Refresh(void);

	int   handler_;
	IOMap ios_;
};

# endif /* LIBS_MULTIPLEX_MULTIPLEX_HPP_ */
