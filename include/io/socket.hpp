#ifndef SOCKET_HPP
#define SOCKET_HPP

# include <iostream>
# include <cerrno>
# include <cstring>


# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <unistd.h>
# include <poll.h>
# include <fcntl.h>


class Server
{
	private:
		int _listen_fd;
		sockaddr_in _addr;//register socket by its address (IP + port)
		pollfd _pfd;
	public:
		Server();
		~Server();

		int start();
		void run();
};

#endif