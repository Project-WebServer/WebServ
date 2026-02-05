#include "socket.hpp"
//socket() → bind() → listen() → accept() → poll()


int main (void)
{
	int yes = 1;
	errno = 0;
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0)
	{
		std::cerr << "socket() failed: " << std::strerror(errno) << std::endl;
		return (1);
	}
	;
	std::cout << "socket fd = " << fd << std::endl;
	close(fd);
	return (0);
}