#include "delete.hpp"

void	Delete::run()
{
	int	http_status = kAccepted; // <<
	std::string fileName = "fileName.txt"; // <<

	if (std::remove(fileName) != 0)
		throw std::runtime_error(strerror(errno));
	http_status = kNoContent;
}