#include "../../include/io/Server.hpp"
#include <sstream>

Server::Server() {}

Server::~Server() {}

void Server::_addListenFd(int fd)
{
	pollfd pfd;
	pfd.fd = fd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	_pfds.push_back(pfd);
}

void Server::run ()
{
	int ready;

	while (g_running)
	{
		ready = poll(_pfds.data(), _pfds.size(), 1000);
		if(!g_running)
			break;
		if(ready < 0)
		{
			if(errno == EINTR)
			{
				if(!g_running)
					break;
				continue;
			}
			std::cerr << "poll() failed: " << std::strerror(errno) << std::endl;
			_cleanupServer();
			return;
		}
		_checkTimeout();
		if(ready == 0)
			continue;
		for (size_t i = 0; i < _pfds.size(); )
		{
			bool removed = false;
			if (_pfds[i].revents & (POLLERR | POLLHUP | POLLNVAL))
			{
				if(_isListenFd(_pfds[i].fd))
				{
					_cleanupServer();
					return;
				}
				if(_pipe_to_client.count(_pfds[i].fd))
					removed = _handleCgiReadable(i);
				else
					removed = _handleClientError(i);
			}	
			else if (_pfds[i].revents & POLLIN)
			{
				if (_isListenFd(_pfds[i].fd))
				{
					_acceptClients(_pfds[i].fd);
					break;
				}
				else if (_pipe_to_client.count(_pfds[i].fd))
					removed = _handleCgiReadable(i);
				else
				{
					Connection &c = _conns[_pfds[i].fd];
					if(c.state != CGI_READING)
						removed = _handleClientReadable(i);
				}
			}	
			else if(_pfds[i].revents & POLLOUT)
				removed = _handleClientWritable(i);
			if(!removed)
				i++;
		}
	}
	std::cout << std::endl;
	std::cout << "Server shutting down..." << std::endl;
	_cleanupServer();
}

