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
}