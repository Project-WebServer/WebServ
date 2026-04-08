#include "../../include/io/Server.hpp"
#include <sys/wait.h>

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
			_conns[c_fd].request.setMaxBodySize(_fd_to_max_body[listen_fd]);
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
		if (_pipe_to_client.count(_pfds[i].fd))
		{
			int client_fd = _pipe_to_client[_pfds[i].fd];
    		Connection &c = _conns[client_fd];
    		if (now - c.cgi.started_at > 30)
    		{
    		    std::cout << "CGI timeout: killing pid " << c.cgi.pid << std::endl;
    		    kill(c.cgi.pid, SIGKILL);
    		    waitpid(c.cgi.pid, NULL, 0);
    		    c.out_buf = "HTTP/1.1 504 Gateway Timeout\r\nContent-Length: 0\r\n\r\n";
				std::cerr << "[TIMEOUT] out_buf size=" << c.out_buf.size() << std::endl;
    		    c.state = SENDING_RESPONSE;
    		    _pipe_to_client.erase(_pfds[i].fd);
    		    _removeFd(i);
    		    for (size_t j = 0; j < _pfds.size(); j++)
    		    {
					std::cerr << "[TIMEOUT] _pfds[" << j << "].fd=" << _pfds[j].fd << std::endl;
    		        if (_pfds[j].fd == client_fd)
    		        {
						std::cerr << "[TIMEOUT] found! setting POLLOUT" << std::endl;
    		            _pfds[j].events = POLLOUT;
    		            break;
    		        }
    		    }
    		    continue;
    		}
    		i++;
    		continue;
			// i++;
			// continue;
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