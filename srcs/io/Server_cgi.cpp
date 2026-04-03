#include "../../include/io/Server.hpp"

void Server::_launchCgi(size_t indx)
{
	int fd = _pfds[indx].fd;
	Connection &c = _conns[fd];

	int pipe_in[2];
	int pipe_out[2];

	if(pipe(pipe_in) == -1 || pipe(pipe_out) == -1)
	{
		c.out_buf = "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 0\r\n\r\n";
		c.state = SENDING_RESPONSE;
		_pfds[indx].events = POLLOUT;
		return;
	}
	pid_t pid = fork();
	if(pid == -1)
	{
		c.out_buf = "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 0\r\n\r\n";
		c.state = SENDING_RESPONSE;
		_pfds[indx].events = POLLOUT;
		return;
	}
	if(pid == 0)
	{
		if(dup2(pipe_in[0], STDIN_FILENO) == -1 || dup2(pipe_out[1], STDOUT_FILENO) == -1)
		{
			close(pipe_in[0]);
			close(pipe_in[1]);
			close(pipe_out[0]);
			close(pipe_out[1]);
			exit(1);
		}
		close(pipe_in[0]);
		close(pipe_in[1]);
		close(pipe_out[0]);
		close(pipe_out[1]);

		char *argv[] = {
			(char *)c.cgi.cgi_path.c_str(),
			(char *)c.cgi.cgi_script.c_str(),
			NULL
		};
		char *env[] = { NULL };
		if(execve(argv[0], argv, env) == -1)
			exit(1);
	}
	close(pipe_in[0]);
	close(pipe_out[1]);

	if(!c.request.getBody().empty())
		write(pipe_in[1], c.request.getBody().data(), c.request.getBody().size());
	close(pipe_in[1]);

	c.is_cgi = true;
	c.state = CGI_READING;
	c.cgi.pid = pid;
	c.cgi.pipe_read_fd = pipe_out[0];
	c.cgi.started_at = time(NULL);
	c.cgi.cgi_buf = "";

	pollfd pfd;
	pfd.events = POLLIN;
	pfd.fd = pipe_out[0];;
	pfd.revents = 0;

	_pfds.push_back(pfd);
	_pipe_to_client[pipe_out[0]] = fd;//map client socket iin poll pool with actuall end of pipe with cgi info
}