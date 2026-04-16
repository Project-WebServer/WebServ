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
				c.request.statusCode(feedReturn::ERROR);
				HandlerResult result = responseHandler(c.request, _conf);
				c.out_buf = result.response;
    		    //c.out_buf = "HTTP/1.1 504 Gateway Timeout\r\nContent-Length: 0\r\n\r\n";
				//std::cerr << "[TIMEOUT] out_buf size=" << c.out_buf.size() << std::endl;
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