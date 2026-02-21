#ifndef SOCKET_HPP
#define SOCKET_HPP

# include <iostream>
# include <cerrno>
# include <cstring>
# include <vector>
# include <map>
# include <set>

# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <unistd.h>
# include <poll.h>
# include <fcntl.h>


class Server
{
	private:
		struct Connection
		{
			std::string in_buf;
		};

		int _listen_fd;
		sockaddr_in _addr;//register socket by its address (IP + port)
		std::vector<pollfd> _pfds;
		// std::set<int> _clients;
		std::map<int, Connection> _conns;

		void _addListenFd();
		void _acceptClients();
		void _removeFd(size_t indx);//should recieve some index from array
		int _setNonBlocking(int fd);
		bool _isListenFd(size_t indx) const;

	public:
		Server();
		~Server();

		int start();
		void run();
		
};



#endif