#include "../../include/io/Server.hpp"
#include <sys/wait.h>

void Server::_checkTimeout()
{
	time_t now = time(NULL);
	for (size_t i = 0; i < _pfds.size(); )
	{
		if(_isListenFd(_pfds[i].fd))
			i++;
		else if (_pipe_to_client.count(_pfds[i].fd))// 1 or 0 
		{
			if(!_CGITimeout(i, now))
				i++;
		}
		else if(!_clientTimeout(i, now))
			i++;
	}
}

bool Server::_CGITimeout(size_t i, time_t now)
{
	int client_fd = _pipe_to_client[_pfds[i].fd];
	if (_conns.find(client_fd) == _conns.end())
	{
		_pipe_to_client.erase(_pfds[i].fd);
		_removeFd(i);
		return true;
	}
	else
	{
    	Connection &c = _conns[client_fd];
		c.last_activity = time(NULL);
    	if (now - c.cgi.started_at > 30)
    	{
    	    std::cout << "CGI timeout: killing pid " << c.cgi.pid << std::endl;
    	    kill(c.cgi.pid, SIGKILL);
    	    waitpid(c.cgi.pid, NULL, 0);
			c.request.setStatusCode(feedReturn::CGI_TIMEOUT);
			HandlerResult result = responseHandler(c.request, _conf);
			c.out_buf = result.response;
    	    c.state = CLOSING;
    	    _pipe_to_client.erase(_pfds[i].fd);
    	    _removeFd(i);
    	    for (size_t j = 0; j < _pfds.size(); j++)
    	    {
    	        if (_pfds[j].fd == client_fd)
    	        {
    	            _pfds[j].events = POLLOUT;
    	            break;
    	        }
    	    }
			return true;
    	}
		else
    		return false;
	}
}

bool Server::_clientTimeout(size_t i, time_t now)
{
	Connection &c = _conns[_pfds[i].fd];
	if(c.state == CGI_READING)
	{
		return false;
	}
	if (now - c.last_activity > 30)
	{
		std::cout << "Timeout: closing fd " << _pfds[i].fd << std::endl;
		_removeFd(i);
		return true;
	}
	return false;
}