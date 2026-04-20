#include "../../include/io/Server.hpp"
#include <sys/wait.h>

void Server::_removeFd(size_t indx)
{
	int fd = _pfds[indx].fd;

	close(fd);
	_conns.erase(fd);
	_pfds.erase(_pfds.begin() + indx);
}

void Server::_resetConnection(Connection &c)
{
	c.in_buf.clear();
	c.out_buf.clear();
	c.state = READING_REQUEST;
	c.keep_alive = false;
}

bool Server::_isListenFd(int fd) const
{
    for (size_t i = 0; i < _listen_fds.size(); i++)
    {
        if (_listen_fds[i] == fd)
            return true;
    }
    return false;
}

void Server::_cleanupServer()
{
	for (size_t i = 0; i < _pfds.size(); i++)
	    close(_pfds[i].fd);
	_pfds.clear();
	for (std::map<int, Connection>::iterator it = _conns.begin(); it != _conns.end(); it++)
		if(it->second.cgi.pid > 0)
		{
			kill(it->second.cgi.pid, SIGKILL);
			waitpid(it->second.cgi.pid, NULL, 0);
		}
	_conns.clear();
}
