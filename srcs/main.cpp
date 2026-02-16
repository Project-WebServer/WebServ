#include "include/io/socket.hpp"


int main()
{
	Server s;

	if(s.start() != 0)
		return (1);
	s.run();
	return (0);
}