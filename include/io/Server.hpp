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
 #include "../config/WebservConf.hpp"


class Server
{
	private:

		// int _listen_fd;
		std::vector<int>			_listen_fds;
		// sockaddr_in					_addr;//register socket by its address (IP + port)
		std::vector<pollfd>			_pfds; //listen fd
		std::map<int, Connection>	_conns; //key = client fd
		WebservConf					_conf;

		void _addListenFd(int fd);
		void _acceptClients(int listen_fd);
		void _removeFd(size_t indx);//should recieve some index from array
		bool _isListenFd(int fd) const;
		void _checkTimeout();
		void _resetConnection(Connection &c);

		int _createListenSocket(uint32_t ip, int port);

		void _logRecv(int fd, ssize_t n) const;// debbug
		void _buildResponse(size_t indx);// debbug

		void _handleListenReadable(int listen_fd);
		void _handleClientReadable(size_t indx);
		void _handleClientWritable(size_t indx);
		void _handleClientError(size_t indx);
		
		

	public:
		Server();
		~Server();

		int start(const WebservConf &conf);
		void run();
};


#endif