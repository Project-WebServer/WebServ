#include "../../include/io/Server.hpp"

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
			_conns[fd].in_buf.append(buf, n);
			c.last_activity = time(NULL);
			//--------temporary------//
			_logRecv(fd, n);
			_buildResponse(indx);
			return;
			//--------temporary------//
			//this is the place to call http parser which read from in_buf <===============
			//ParseResult result = HttpParser::feed(c.in_buf, c.request) (NEED_MORE, COMPLETE, ERROR(enum???)// do i realy need an reques here
			//if(result = NEED_MORE)
			//	continue;
			//if(result = ERROR)
			// {
            //     track B said thad request is not valid
            //     send 400 Bad Request and close
            //     c.out_buf = HttpResponse::makeError(400);
            //     c.state = CLOSING;
            //     _pfds[indx].events = POLLOUT;
            //     return;
            // }
			// if (result == COMPLETE)
            // {
            	// --- Pass to track C---
            	// Router::handle() return serialized answer as a string
				// c.out_buf = Router::handle(c.request);
				//c.keep_alive = c.request.isKeepAlive();
				//c.state = SENDING_RESPONSE;
				//_pfds[indx].events = POLLOUT;
				//return;
			//}
		}
		else if(n == 0)
		{
			std::cout << "fd " << fd << " closed by client" << std::endl;
			_removeFd(indx);
			return;
		}
		else
		{
			std::cerr << "recv() error on fd " << fd << std::endl;
			_removeFd(indx);
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