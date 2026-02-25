#ifndef SERVER_HPP
#define SERVER_HPP

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

# include "Connection.hpp"

class Server
{
	private:

		int _listen_fd;
		sockaddr_in _addr;//register socket by its address (IP + port)
		std::vector<pollfd> _pfds; //listen fd
		std::map<int, Connection> _conns; //key = client fd

		void _addListenFd();
		void _acceptClients();
		void _removeFd(size_t indx);//should recieve some index from array

		void _logRecv(int fd, ssize_t n) const;// debbug
		void _buildResponse(size_t indx);
		void _handleListenReadable();
		void _handleClientReadable(size_t indx);
		void _handleClientWritable(size_t indx);
		void _handleClientError(size_t indx);

		void _resetConnection(Connection &c);
		// int _setNonBlocking(int fd);
		// bool _isListenFd(size_t indx) const;

	public:
		Server();
		~Server();

		int start();
		void run();
		
};


#endif