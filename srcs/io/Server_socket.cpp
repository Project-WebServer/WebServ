#include "../../include/io/Server.hpp"

int Server::start(const WebservConf &conf)
{
	_conf = conf;
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
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd < 0)
	{
		std::cerr << "socket() failed: " << std::strerror(errno) << std::endl;
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

	if(bind(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0)
	{
		std::cerr << "bind() failed: " << std::strerror(errno) << std::endl;
		close(fd);
		return (-1);
	}
	if(listen(fd, 128) < 0)
	{
		std::cerr << "listen() failed: " << std::strerror(errno) << std::endl;
		close(fd);
		return (-1);
	}
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
	std::cout << "Server listening on " << inet_ntoa(addr.sin_addr) << ":" << port << " (fd=" << fd << ")" << std::endl;
	return (fd);
}