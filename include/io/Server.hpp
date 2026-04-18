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
# include <poll.h>// for pollfd struct
# include <fcntl.h>
#include <csignal>

# include "Connection.hpp"
#include "../config/WebservConf.hpp"
#include "../../include/handlers/Response.hpp"

extern volatile sig_atomic_t g_running;
void	setupSignals();

class Server
{
	private:

		std::vector<int>			_listen_fds;
		std::vector<pollfd>			_pfds; //listen fd
		std::map<int, Connection>	_conns; //key = client fd
		WebservConf					_conf;
		std::map<int, int>			_pipe_to_client;
		std::map<int, size_t>		_fd_to_max_body;

		void _addListenFd(int fd);
		void _removeFd(size_t indx);//should recieve some index from array
		bool _isListenFd(int fd) const;
		void _checkTimeout();
		void _resetConnection(Connection &c);

		int _createListenSocket(uint32_t ip, int port);

		void _acceptClients(int listen_fd);
		bool _handleClientReadable(size_t indx);
		bool _handleClientWritable(size_t indx);
		bool _handleClientError(size_t indx);
		void _launchCgi(size_t indx);
		bool _finishCgi(size_t indx, int client_fd, const std::string &response);
		std::string _buildCgiResponse(const std::string &cgi_buf, Connection &c);
		bool _handleCgiReadable(size_t indx);
		void _cleanupServer();
		

	public:
		Server();
		~Server();

		int start(const WebservConf &conf);
		void run();
};


#endif