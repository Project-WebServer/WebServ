#include "../../include/io/Server.hpp"
#include <sys/wait.h>
#include <sys/resource.h>

// обмежуємо CPU час для CGI процесу


static std::string buildCgiResponse(const std::string &cgi_buf);
static std::string normalizeCgiOutput(const std::string &raw);

void Server::_launchCgi(size_t indx)
{
	int fd = _pfds[indx].fd;
	Connection &c = _conns[fd];
	std::string script = "./data" + c.request.getPath();//patch

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
		// for (int i = 3; i < 1024; i++)
        // 	close(i);
		close(pipe_in[0]);
		close(pipe_in[1]);
		close(pipe_out[0]);
		close(pipe_out[1]);

		struct rlimit rl;
		rl.rlim_cur = 10; // 10 секунд CPU
		rl.rlim_max = 10;
		setrlimit(RLIMIT_CPU, &rl);
		
		char *argv[] = {
			(char *)c.cgi.cgi_path.c_str(),
			(char *)script.c_str(),
			NULL
		};
		char *env[] = { NULL };
		if(execve(argv[0], argv, env) == -1)
		{
			exit(1);
		}
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

void Server::_handleCgiReadable(size_t indx)
{
	int pipe_fd = _pfds[indx].fd;
	int client_fd = _pipe_to_client[pipe_fd];
	Connection &c = _conns[client_fd];

	char buf[4096];
	ssize_t n = read(pipe_fd, buf, sizeof(buf));
	if (n > 0)
	{
		c.cgi.cgi_buf.append(buf, n);
	}
	else if (n == 0)
	{
		waitpid(c.cgi.pid, NULL, 0);
		c.out_buf = buildCgiResponse(c.cgi.cgi_buf);
		c.state = SENDING_RESPONSE;
		_pipe_to_client.erase(pipe_fd);
		_removeFd(indx);

		for (size_t i = 0; i < _pfds.size(); i++)
		{
			if (_pfds[i].fd == client_fd)
			{
				_pfds[i].events = POLLOUT;
				break;
			}
		}
	}
	else
	{
		waitpid(c.cgi.pid, NULL, 0);
		c.out_buf = "HTTP/1.1 502 Bad Gateway\r\nContent-Length: 0\r\n\r\n";
		c.state = SENDING_RESPONSE;
		_pipe_to_client.erase(pipe_fd);
		_removeFd(indx);
		for (size_t i = 0; i < _pfds.size(); i++)
		{
			if (_pfds[i].fd == client_fd)
			{
				_pfds[i].events = POLLOUT;
				break;
			}
		}
	}
}

static std::string normalizeCgiOutput(const std::string &raw)
{
    std::string result;
    for (size_t i = 0; i < raw.size(); i++)
    {
        if (raw[i] == '\n' && (i == 0 || raw[i - 1] != '\r'))
            result += '\r';
        result += raw[i];
    }
    return result;
}

static std::string buildCgiResponse(const std::string &cgi_buf)
{
    std::string normalized = normalizeCgiOutput(cgi_buf);
    
    size_t separator = normalized.find("\r\n\r\n");
    if (separator == std::string::npos)
        return "HTTP/1.1 502 Bad Gateway\r\nContent-Length: 0\r\n\r\n";
    
    std::string cgi_headers = normalized.substr(0, separator);
    std::string body = normalized.substr(separator + 4);
    
    return "HTTP/1.1 200 OK\r\n" 
        + cgi_headers + "\r\n"
        + "Content-Length: " + std::to_string(body.size()) + "\r\n"
        + "\r\n"
        + body;
}