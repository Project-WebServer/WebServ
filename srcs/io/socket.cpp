#include "socket.hpp"
//socket() → bind() → listen() → accept() → poll()


int main (void)
{
	int yes = 1;
	int sfd = socket(AF_INET, SOCK_STREAM, 0);//kernel socket object. make struct for args, to make it more universal
	if(sfd < 0)
	{
		std::cerr << "socket() failed: " << std::strerror(errno) << std::endl;//write handle error func
		return (1);
	}
	if(setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0)
	{
		std::cerr << "setsockopt(SO_REUSEADDR) failed: " << std::strerror(errno) << std::endl;
		close(sfd);
		return (1);
	}

	sockaddr_in addr;//register socket by its address (IP + port)
	std::memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(8080);

	//connect socket to IP:PORT
	//int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

	if(bind(sfd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0)
	{
		std::cerr << "bind() failed: " << std::strerror(errno) << std::endl;
		close(sfd);
		return (1);
	}
	//this sfd can listen up to 128 client and put them in a queue before accept() and said that its a server not client
	if(listen(sfd, 128) < 0)
	{
		std::cerr << "listen() failed: " << std::strerror(errno) << std::endl;
		close(sfd);
		return (1);
	}

	std::cout << "socket fd = " << sfd << std::endl;
	std::cout << "listening to fd = " << sfd << std::endl;

	while (true) pause();

	close(sfd);
	return (0);
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