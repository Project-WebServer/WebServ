#include "../../include/io/Server.hpp"
#include "../../include/http/request.hpp"
#include "../../include/handlers/Response.hpp"

void Server::_handleClientReadable(size_t indx)
{
	int fd = _pfds[indx].fd;
	Connection &c = _conns[fd];
	char buf[4096];
	while(true)
	{
		ssize_t n = recv(fd, buf, sizeof(buf), 0);
		if(n > 0)
		{
			//_conns[fd].in_buf.append(buf, n);
			c.last_activity = time(NULL);
			_logRecv(fd, n);
			feedReturn parseResult = c.request.feed(std::string(buf, n));
			if (parseResult == feedReturn::MAX_BODY_SIZE)
			{
			    c.out_buf = "HTTP/1.1 413 Payload Too Large\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";
			    c.state = CLOSING;
			    _pfds[indx].events = POLLOUT;
			    return;
			}
			// std::cout << "parser buffer size = " << c.request.bufferSize() << std::end;
			//this is the place to call http parser which read from in_buf <===============
			//c.in_buf.erase(0, consumed);// delete consumed by parser bytes
			if (parseResult == feedReturn::COMPLETE)
            {
				//chekc for the limits
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
				return;
			}
			if(parseResult == feedReturn::INCOMPLETE)
				continue;
			if(parseResult == feedReturn::ERROR)
			{
				HandlerResult handlerResult = responseHandler(c.request, _conf);
				c.out_buf = handlerResult.response;
				c.state = CLOSING;
				_pfds[indx].events = POLLOUT;
				return;
			}
		}
		else if(n == 0)
		{
			std::cout << "fd " << fd << " closed by client" << std::endl;
			_removeFd(indx);
			return;
		}
		else if(n < 0)
		{
			std::cerr << "recv() error on fd " << fd << std::endl;
			// _removeFd(indx);
			return;
		}
	}
}

void Server::_handleClientWritable(size_t indx)
{
	int fd = _pfds[indx].fd;
	Connection &c = _conns[fd];

	if(c.out_buf.empty())
	{
		if(c.state == CLOSING)
		{
			_removeFd(indx);
			return;
		}
		_resetConnection(c);
		_pfds[indx].events = POLLIN;
		return;
	}
	ssize_t n = send(fd, c.out_buf.data(), c.out_buf.size(), 0);//bytes
	if(n < 0)
	{
		_removeFd(indx);
		return;
	}
	if(n == 0)
	{
		_removeFd(indx);
		return;
	}
	c.out_buf.erase(0, n);
	if (c.out_buf.empty())
	{
		if(c.state == CLOSING || !c.keep_alive)
		{
			_removeFd(indx);
			return;
		}
		_resetConnection(c);
		_pfds[indx].events = POLLIN;
	}
}

void Server::_handleListenReadable(int listen_fd)
{
	_acceptClients(listen_fd);
}

void Server::_handleClientError(size_t indx)
{
	_removeFd(indx);
}