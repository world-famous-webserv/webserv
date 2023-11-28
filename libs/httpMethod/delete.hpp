#ifndef LIBS_DELETE_HPP
# define LIBS_DELETE_HPP
# include <errno>
# include <cstdio>
# include <string>
# include "http_code.hpp"

class Delete
{
private:
	Delete();
	~Delete();
	Delete(const Delete &obj);
	Delete & operator = (const Delete &obj);
public:
	static void	run();
};

#endif /* LIBS_DELETE_HPP */