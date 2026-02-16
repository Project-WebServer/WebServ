#include "include/io/socket.hpp"
//socket() → bind() → listen() → accept() → poll()

Server::Server() : _listen_fd(-1)
{
	std::memset(&_addr, 0, sizeof(_addr));
	std::memset(&_pfd, 0, sizeof(_pfd));
}

Server::~Server()
{
	if(_listen_fd >= 0)
		close(_listen_fd);
}

int Server::start()
{
	int yes = 1;
	_listen_fd = socket(AF_INET, SOCK_STREAM, 0);//kernel socket object. make struct for args, to make it more universal
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
	//int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

	if(bind(_listen_fd, reinterpret_cast<sockaddr*>(&_addr), sizeof(_addr)) < 0)
	{
		std::cerr << "bind() failed: " << std::strerror(errno) << std::endl;
		return (1);
	}
	//this sfd can listen up to 128 client and put them in a queue before accept() and said that its a server not client
	if(listen(_listen_fd, 128) < 0)
	{
		std::cerr << "listen() failed: " << std::strerror(errno) << std::endl;
		return (1);
	}

	_pfd.fd = _listen_fd;
	_pfd.events = POLLIN;
	_pfd.revents = 0;// poll will fill this in

	std::cout << "socket fd = " << _listen_fd << std::endl;
	std::cout << "listening to fd = " << _listen_fd << std::endl;
	return (0);
}

void Server::run ()
{
	int ready;

	while (true)
	{
		ready = poll(&_pfd, 1, -1);//timeout -1 - wait forewer(later 1000 in mcsec)
		if(ready < 0) // > 0 number of fd on which we have events; == 0 if timeout(not in -1 case)
		{
			if(errno == EINTR)
				continue;// for signa interaption - keep waiting, not break the loop;
			std::cerr << "poll() failed" << std::strerror(errno) << std::endl;
			return;
		}
		if (_pfd.revents & (POLLERR | POLLHUP | POLLNVAL)) //checking error flags
			return;
		if (_pfd.revents & POLLIN)// i POLLIN included to the list of event that alredy happened
		{
			std::cout << "ready to accept" << std::endl;
			int cfd = accept(_listen_fd, NULL, NULL);
			if(cfd < 0)
			{
				std::cerr << "accept() failed" << std::strerror(errno) << std::endl;
				return;
			}
			std::cout << "accepted cient fd"<< cfd << std::endl;
			close(cfd);
		}
	}
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