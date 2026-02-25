#include "../../include/io/Server.hpp"
#include <sstream>
//socket() → bind() → listen() → accept() → poll()

Server::Server() : _listen_fd(-1)
{
	std::memset(&_addr, 0, sizeof(_addr));
}

Server::~Server()
{
	if(_listen_fd >= 0)
		close(_listen_fd);
}

void Server::_addListenFd()
{
	pollfd pfd;
	pfd.fd = _listen_fd;
	pfd.events = POLLIN;
	pfd.revents = 0;// poll will fill this in
	_pfds.push_back(pfd);
}

void Server::_acceptClients()
{
	while(true)
	{
		int c_fd = accept(_listen_fd, NULL, NULL);
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
			std::cout << "accepted cient fd"<< c_fd << std::endl;
			continue;
		}
		if(errno == EAGAIN || errno == EWOULDBLOCK)
		{
			std::cout << "no more pending connections" << std::endl;
			break;
		}
		if(errno == EINTR)//repeat accept; was interupted y signal try ones more
		{
			continue;
		}
		if(errno == ECONNABORTED)// clien chanched his mind, ignore and proceed
		{
			continue;
		}
		std::cerr << "accept() failed: " << std::strerror(errno) << std::endl;
		
		break;
	}
}

void Server::_logRecv(int fd, ssize_t n) const
{
	std::cout << "---- fd " << fd 
			<< " | received " << n << " bytes"
			<< " | total in buffer " << _conns.at(fd).bytesReceived() << " bytes"
			<< " ----" << std::endl;

}

void Server::_buildResponse(size_t indx)
{
	int fd = _pfds[indx].fd;
    Connection &c = _conns[fd];

    if (c.state != READING_REQUEST)
        return;
    if (c.in_buf.find("\r\n\r\n") == std::string::npos)
        return;

    std::string body = "received " + c.in_buf;

    std::ostringstream oss;
    oss << "HTTP/1.1 200 OK\r\n"
        << "Content-Type: text/plain\r\n"
        << "Content-Length: " << body.size() << "\r\n"
        << "Connection: close\r\n"
        << "\r\n"
        << body;

    c.out_buf = oss.str();
    c.state = SENDING_RESPONSE;
    _pfds[indx].events = POLLOUT;
}

void Server::_removeFd(size_t indx)
{
	int fd = _pfds[indx].fd;

	close(fd);
	_conns.erase(fd);
	_pfds.erase(_pfds.begin() + indx);
}

void Server::_handleListenReadable()
{
	_acceptClients();
}

void Server::_handleClientError(size_t indx)
{
	_removeFd(indx);
}

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
			_logRecv(fd, n);
			//--------temporary------//
			_buildResponse(indx);
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
		if(n == 0)
		{
			_removeFd(indx);
			return;
		}
		if(n < 0)
		{
			//_removeFd(indx);
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
		return;
	if(c.state == CLOSING || !c.keep_alive)
	{
		_removeFd(indx);
		return;
	}
	_resetConnection(c);
	_pfds[indx].events = POLLIN;
}

void Server::_resetConnection(Connection &c)
{
	c.in_buf.clear();
	c.out_buf.clear();
	c.state = READING_REQUEST;
	c.keep_alive = false;
}

int Server::start()
{
	int yes = 1;
	_listen_fd = socket(AF_INET, SOCK_STREAM, 0);//kernel socket object. make struct for args, to make it more universal
	//add flag NONBLOCKING through fcntl??
	if(_listen_fd < 0)
	{
		std::cerr << "socket() failed: " << std::strerror(errno) << std::endl;//write handle error func
		return (1);
	}
	if(setsockopt(_listen_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0)
	{
		std::cerr << "setsockopt(SO_REUSEADDR) failed: " << std::strerror(errno) << std::endl;
		return (1);
	}

	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	_addr.sin_port = htons(8080);

	//connect socket to IP:PORT
	if(bind(_listen_fd, reinterpret_cast<sockaddr*>(&_addr), sizeof(_addr)) < 0)
	{
		std::cerr << "bind() failed: " << std::strerror(errno) << std::endl;
		return (1);
	}
	//this sfd can listen up to 128 client and put them in a queue before accept() and said that its a server not client
	if(listen(_listen_fd, 128) < 0)// 128 - backlog
	{
		std::cerr << "listen() failed: " << std::strerror(errno) << std::endl;
		return (1);
	}
	//to make listen socket non-blocking
	int flags = fcntl(_listen_fd,F_GETFL, 0);
	if(flags < 0)
	{
		std::cerr << "fcntl(F_GETFL) failed: " << std::strerror(errno) << std::endl;
		return (1);
	}
	if (fcntl(_listen_fd, F_SETFL, flags | O_NONBLOCK) < 0)
	{
		std::cerr << "fcntl(F_SETFL) failed: " << std::strerror(errno) << std::endl;
		return 1;
	}
	_pfds.clear();
	_conns.clear();
	_addListenFd();
	std::cout << "socket fd = " << _listen_fd << std::endl;
	std::cout << "listening to fd = " << _listen_fd << std::endl;
	return (0);
}

void Server::run ()
{
	int ready;

	while (true)
	{
		ready = poll(_pfds.data(), _pfds.size(), 5000);//timeout -1 - wait forewer(later 1000 in mcsec)
		//check timeout here
		if(ready < 0) // > 0 number of fd on which we have events; == 0 if timeout(not in -1 case)
		{
			if(errno == EINTR)
				continue;// for signal interaption - keep waiting, not break the loop;
			std::cerr << "poll() failed: " << std::strerror(errno) << std::endl;
			return;
		}
		if (_pfds[0].revents & (POLLERR | POLLHUP | POLLNVAL)) //checking error flags
			return;
		if (_pfds[0].revents & POLLIN)// i POLLIN included to the list of event that alredy happened
			_handleListenReadable();
		for (size_t i = 1; i < _pfds.size(); )
		{
			if (_pfds[i].revents & (POLLERR | POLLHUP | POLLNVAL))
			{
				_handleClientError(i);
				continue;
			}
			if(_pfds[i].revents & POLLIN)
			{
				_handleClientReadable(i);
				i++;
				continue;
			}
			if(_pfds[i].revents & POLLOUT)
			{
				_handleClientWritable(i);
				continue;
			}
			i++;
		}
	}
	//managr cgi fds somewhere here in run
}


//{
//open listening fd
//open client fd
//read something to a buffer
//http check buffer for the end of the header \r\n\r\n
//if header is full, figure out is there any body and how long is it through Content-Length
//give it further and figure out what to do with that
//if not full tells that thre are not enought bytes -> need more data
//socket read new chank of info throu the client fd to the in_buffer
//http checks ones more....
//when server returns HTTP responce (red /index.html; redirect->new adress; error 404, 403, 405; CGI->run the program)
//http take the status (200, 404...), length of header, body(Content-Length) and put it together into http response
//and stores it into my outbuffer
//o/i send it back to the client from out_buffer by poll
//when last chank sended -> close client fd(socket?)
//}


/*
{
1)Start server
- create Listening socket (fd)
- bind + listen
- add listening fd into poll (POLLIN)
2)new client
- poll show POLLIN on listening fd
- accept() -> client fd
- make client fd non-blocking
- add client fd into poll (POLLIN)
- make Connection {in_buffer, out_buffer, state = READING}
3)read request
- poll showed POLLIN on client fd
- recv() -> add bytes to the in_buffer
- HTTP parser checks:
	a) weather \r\n\r\n (end oh header present)
	b) if yes - do we need to add body and how much
- if "need more data"  -> wait for the next POLLIN
4)request is ready
- HTTP engine prepared HttpRequest
- Server logic decide who it is: file/redirect/ error/ CGI
- recieve HttpResponse (status +headers + body)
5)send response
- HTTP engine serialize HttpResponse into bytes
- put them in out_buffer
- O/I put poll events: POLLOUT (if out_bufer not empty - POLLOUT, if empty - POLLIN)
- poll showed POLLOUT -> send() from out_buffer
- untill out_buffer become empty
6)after response
- if close -> clean poll + close(client fd) + delete Connection
- if keep-alive -> clear in_buffer, state=READING, leave fd and wait for new request
}
*/