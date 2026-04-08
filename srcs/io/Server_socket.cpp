#include "../../include/io/Server.hpp"

int Server::start(const WebservConf &conf)
{
	_conf = conf;
	_pfds.clear();
	_conns.clear();

	const std::vector<ENDPOINT> &endpoints = conf.getAvailableEndPoints();

	for (size_t i = 0; i < endpoints.size(); i++)
	{
		int fd = _createListenSocket(endpoints[i].ip, endpoints[i].port);
		if (fd < 0)
			return (1);
		_listen_fds.push_back(fd);
		const std::vector<ServerConf> *servers = conf.matchServer(endpoints[i].ip, endpoints[i].port);
		if (servers && !servers->empty())
		    _fd_to_max_body[fd] = servers->front().getClientSize();
		_addListenFd(fd);
	}
	return 0;
}

int Server::_createListenSocket(uint32_t ip, int port)
{
	int yes = 1;
	int fd = socket(AF_INET, SOCK_STREAM, 0);//kernel socket object. make struct for args, to make it more universal
	//add flag NONBLOCKING through fcntl??
	if(fd < 0)
	{
		std::cerr << "socket() failed: " << std::strerror(errno) << std::endl;//write handle error func
		return (-1);
	}
	if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0)
	{
		std::cerr << "setsockopt(SO_REUSEADDR) failed: " << std::strerror(errno) << std::endl;
		close(fd);
		return (-1);
	}
	sockaddr_in addr;
	std::memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(ip);
	addr.sin_port = htons(port);

	//connect socket to IP:PORT
	if(bind(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0)
	{
		std::cerr << "bind() failed: " << std::strerror(errno) << std::endl;
		close(fd);
		return (-1);
	}
	//this sfd can listen up to 128 client and put them in a queue before accept() and said that its a server not client
	if(listen(fd, 128) < 0)// 128 - backlog
	{
		std::cerr << "listen() failed: " << std::strerror(errno) << std::endl;
		close(fd);
		return (-1);
	}
	//to make listen socket non-blocking
	int flags = fcntl(fd, F_GETFL, 0);
	if(flags < 0)
	{
		std::cerr << "fcntl(F_GETFL) failed: " << std::strerror(errno) << std::endl;
		close(fd);
		return (-1);
	}
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0)
	{
		std::cerr << "fcntl(F_SETFL) failed: " << std::strerror(errno) << std::endl;
		close(fd);
		return -1;
	}
	std::cout << "listening to fd = " << fd << std::endl;
	return (fd);
}