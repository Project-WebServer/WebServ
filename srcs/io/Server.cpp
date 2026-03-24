#include "../../include/io/Server.hpp"
#include <sstream>
//socket() → bind() → listen() → accept() → poll()

Server::Server() {}

Server::~Server()
{
	for (size_t i = 0; i < _listen_fds.size(); i++)
		close(_listen_fds[i]);
}

void Server::_addListenFd(int fd)
{
	pollfd pfd;
	pfd.fd = fd;
	pfd.events = POLLIN;
	pfd.revents = 0;// poll will fill this in
	_pfds.push_back(pfd);
}

void Server::run ()
{
	int ready;

	while (g_running)
	{
		ready = poll(_pfds.data(), _pfds.size(), 5000);//timeout -1 - wait forewer(later 1000 in mcsec)
		//check every 5 sec max imit is 30 sec
		if(!g_running)
			break;
		if(ready < 0) // > 0 number of fd on which we have events; == 0 if timeout(not in -1 case)
		{
			if(errno == EINTR)
			{
				if(!g_running)
					break;
				continue;// for signal interaption - keep waiting, not break the loop;
			}
			std::cerr << "poll() failed: " << std::strerror(errno) << std::endl;
			return;
		}
		_checkTimeout();
		if(ready == 0)//timeout
			continue;
		for (size_t i = 0; i < _pfds.size(); )
		{
			if (_pfds[i].revents & (POLLERR | POLLHUP | POLLNVAL)) //checking error flags
			{
				if(_isListenFd(_pfds[i].fd))
					return;// have to stop poll cos it will always return an error for this fd
				_handleClientError(i);
				continue;
			}	
			if (_pfds[i].revents & POLLIN)// i POLLIN included to the list of event that alredy happened
			{
				if (_isListenFd(_pfds[i].fd))
					_handleListenReadable(_pfds[i].fd);
				else
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

	std::cout << "server shutting down..." << std::endl;
	for (size_t i = 0; i < _pfds.size(); i++)
	    close(_pfds[i].fd);
	_pfds.clear();
	_conns.clear();
	//managr cgi fds somewhere here in run
}


//---------------------temporary-----------------------//

void Server::_logRecv(int fd, ssize_t n) const
{
	std::cout << "---- fd " << fd 
			<< " | received " << n << " bytes"
			// << " | total in buffer " << _conns.at(fd).bytesReceived() << " bytes"
			<< " ----" << std::endl;
}

void Server::_buildResponse(size_t indx)
{
	int fd = _pfds[indx].fd;
    Connection &c = _conns[fd];

    if (c.state != READING_REQUEST)
		return;
	
	std::string body = "path: " + c.request.getPath();
    // if (c.in_buf.find("\r\n\r\n") == std::string::npos)
    //     return;

    // std::string body = "received " + c.in_buf;

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

//---------------------temporary-----------------------//

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