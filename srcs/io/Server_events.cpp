#include "../../include/io/Server.hpp"
#include "../../include/http/request.hpp"
#include "../../include/handlers/Response.hpp"
#include <sys/wait.h>
#include <sys/resource.h>

bool Server::_handleClientReadable(size_t indx)
{
	int fd = _pfds[indx].fd;
	Connection &c = _conns[fd];
	char buf[4096];
	while(true)
	{
		ssize_t n = recv(fd, buf, sizeof(buf), 0);
		if(n > 0)
		{
			c.last_activity = time(NULL);
			feedReturn parseResult = c.request.feed(std::string(buf, n));
			//printf("[DEBUG] feed return: %d\n", parseResult);
			if (parseResult == feedReturn::MAX_BODY_SIZE)
			{
				c.request.setStatusCode(feedReturn::MAX_BODY_SIZE);
			    HandlerResult handlerResult = responseHandler(c.request, _conf);
				c.out_buf = handlerResult.response;
			    c.state = CLOSING;
			    _pfds[indx].events = POLLOUT;
			    return false;
			}
			if (parseResult == feedReturn::COMPLETE)
            {

				HandlerResult handlerResult = responseHandler(c.request, _conf);
				if(handlerResult.is_cgi)
				{
					c.cgi.cgi_path = handlerResult.cgi_path;
					c.cgi.cgi_script = handlerResult.cgi_script;
					c.state = CGI_READING;
					_launchCgi(indx);
				}
				else
				{
					c.out_buf = handlerResult.response;
					c.state = SENDING_RESPONSE;
					_pfds[indx].events = POLLOUT;
				}
				return false;
			}
			if(parseResult == feedReturn::INCOMPLETE)
				continue;
			if(parseResult == feedReturn::NO_HOST_ERROR)
			{
				c.request.setStatusCode(feedReturn::NO_HOST_ERROR);
			    HandlerResult handlerResult = responseHandler(c.request, _conf);
				c.out_buf = handlerResult.response;
			    c.state = CLOSING;
			    _pfds[indx].events = POLLOUT;
			    return false;
			}
			if (parseResult == feedReturn::UNSUPPORTED_HTTP)
			{
			    c.request.setStatusCode(feedReturn::UNSUPPORTED_HTTP);
			    HandlerResult handlerResult = responseHandler(c.request, _conf);
			    c.out_buf = handlerResult.response;
			    c.state = CLOSING;
			    _pfds[indx].events = POLLOUT;
			    return false;
			}
			if (parseResult == feedReturn::EXPECT_FAILED || parseResult == feedReturn::METHOD_ERROR)
			{
			    c.request.setStatusCode(parseResult);
			    HandlerResult handlerResult = responseHandler(c.request, _conf);
			    c.out_buf = handlerResult.response;
			    c.state = CLOSING;
			    _pfds[indx].events = POLLOUT;
			    return false;
			}
			if(parseResult == feedReturn::ERROR)
			{
				HandlerResult handlerResult = responseHandler(c.request, _conf);
				c.out_buf = handlerResult.response;
				c.state = CLOSING;
				_pfds[indx].events = POLLOUT;
				return false;
			}
		}
		else if(n <= 0)
		{
			if(c.state == READING_REQUEST && !c.request.getBuffer().empty())
			{
				c.request.setStatusCode(feedReturn::REQUEST_TIMEOUT);
			    HandlerResult handlerResult = responseHandler(c.request, _conf);
			    c.out_buf = handlerResult.response;
			    c.state = CLOSING;
			    _pfds[indx].events = POLLOUT;
			    return false;
			}
			_removeFd(indx);
			return true;
		}
		// else if(n < 0)
		// {
		// 	return false;
		// }
	}
}

bool Server::_handleClientWritable(size_t indx)
{
	int fd = _pfds[indx].fd;
	Connection &c = _conns[fd];
	if(c.out_buf.empty())
	{
		if(c.state == CLOSING)
		{
			_removeFd(indx);
			return true;
		}

		_resetConnection(c);
		_pfds[indx].events = POLLIN;
		return false;
	}
	ssize_t n = send(fd, c.out_buf.data(), c.out_buf.size(), 0);//bytes
	if(n <= 0)
	{
		_removeFd(indx);
		return true;
	}
	c.out_buf.erase(0, n);
	if (c.out_buf.empty())
	{
		if(c.state == CLOSING || !c.keep_alive)
		{
			_removeFd(indx);
			return true;
		}
		_resetConnection(c);
		_pfds[indx].events = POLLIN;
	}
	return false;
}

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
			continue;
		}
		else if(errno == EAGAIN || errno == EWOULDBLOCK)
		{
			// std::cout << "no more pending connections" << std::endl;
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

bool Server::_handleClientError(size_t indx)
{
	int fd = _pfds[indx].fd;
    Connection &c = _conns[fd];

    if (c.cgi.pid > 0)
    {
        kill(c.cgi.pid, SIGKILL);
        waitpid(c.cgi.pid, NULL, 0);
        for (size_t i = 0; i < _pfds.size(); i++)
        {
            if (_pipe_to_client.count(_pfds[i].fd) && _pipe_to_client[_pfds[i].fd] == fd)
            {
                _pipe_to_client.erase(_pfds[i].fd);
                _removeFd(i);
                break;
            }
        }
    }
    _removeFd(indx);
    return true;
}
