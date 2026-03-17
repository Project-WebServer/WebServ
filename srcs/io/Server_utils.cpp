#include "../../include/io/Server.hpp"

void Server::_acceptClients(int listen_fd)
{
	while(true)
	{
		int c_fd = accept(listen_fd, NULL, NULL);
		if(c_fd >= 0)
		{
			int flags = fcntl(c_fd, F_GETFL, 0);//file control help change or read fd settings
			if(flags < 0 || fcntl(c_fd, F_SETFL, flags | O_NONBLOCK) < 0)
			{
				std::cerr << "fcntl() failed: " << std::strerror(errno) << std::endl;
				close(c_fd);
				continue;
			}
			pollfd p;
			p.fd = c_fd;
			p.events = POLLIN;
			p.revents = 0;
			_pfds.push_back(p);
			_conns[c_fd] = Connection();
			_conns[c_fd].last_activity = time(NULL);
			std::cout << "accepted cient fd"<< c_fd << std::endl;
			continue;
		}
		else if(errno == EAGAIN || errno == EWOULDBLOCK)
		{
			std::cout << "no more pending connections" << std::endl;
			break;
		}
		else if(errno == EINTR)//repeat accept; was interupted y signal try ones more
		{
			continue;
		}
		else if (errno == ECONNABORTED)// clien chanched his mind, ignore and proceed
		{
			continue;
		}
		std::cerr << "accept() failed: " << std::strerror(errno) << std::endl;
		break;
	}
}

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

void Server::_checkTimeout()
{
	time_t now = time(NULL);
	for (size_t i = 0; i < _pfds.size(); )
	{
		if(_isListenFd(_pfds[i].fd))
		{
			i++;
			continue;
		}
		Connection &c = _conns[_pfds[i].fd];
		if (now - c.last_activity > 30) // 30 секунд без активності
		{
			std::cout << "timeout: closing fd " << _pfds[i].fd << std::endl;
			_removeFd(i);
			continue;
		}
		i++;
	}
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