#include "../../include/io/Server.hpp"
#include <sys/wait.h>
#include <sys/resource.h>

static std::string normalizeCgiOutput(const std::string &raw);

void Server::_launchCgi(size_t indx)
{
	int fd = _pfds[indx].fd;
	
	Connection &c = _conns[fd];
	std::string script = c.cgi.cgi_script;

	int pipe_in[2];
	int pipe_out[2];

	if(pipe(pipe_in) == -1 || pipe(pipe_out) == -1)
	{
		close(pipe_in[0]);
		close(pipe_out[1]);
		c.request.setStatusCode(feedReturn::INTERNAL_ERROR);
		HandlerResult result = responseHandler(c.request, _conf);
		c.out_buf = result.response;
		c.state = SENDING_RESPONSE;
		_pfds[indx].events = POLLOUT;
		return;
	}
	pid_t pid = fork();

	if(pid == -1)
	{
		c.request.setStatusCode(feedReturn::INTERNAL_ERROR);
		HandlerResult result = responseHandler(c.request, _conf);
		c.out_buf = result.response;
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

		struct rlimit rl;
		rl.rlim_cur = 10;
		rl.rlim_max = 10;
		setrlimit(RLIMIT_CPU, &rl);

		std::string script_dir = script.substr(0, script.rfind('/'));
		std::string script_name = script.substr(script.rfind('/') + 1);
		if (chdir(script_dir.c_str()) == -1)
		{
		    std::cerr << "[CHILD] chdir failed: " << strerror(errno) << std::endl;
		    exit(1);
		}

		char *argv[] = {
			(char *)c.cgi.cgi_path.c_str(),
			(char *)script_name.c_str(),
			NULL
		};
		std::string contentLenght = "CONTENT_LENGTH=" + std::to_string(c.request.getBody().size());
		std::string requestMethod = "REQUEST_METHOD=" + c.request.getMethodStr();
		std::string contentType = "CONTENT_TYPE=" + c.request.getContType();
		std::string servProtocol = "SERVER_PROTOCOL=" + c.request.getVersionStr();

		char *env[] = {
			(char*)contentLenght.c_str(),
			(char*)requestMethod.c_str(),
			(char*)contentType.c_str(),
			(char*)servProtocol.c_str(),
			NULL };
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
	pfd.fd = pipe_out[0];
	pfd.revents = 0;

	_pfds.push_back(pfd);
	_pipe_to_client[pipe_out[0]] = fd;//map client socket iin poll pool with actuall end of pipe with cgi info
}

bool Server::_handleCgiReadable(size_t indx)
{
	int pipe_fd = _pfds[indx].fd;
	int client_fd = _pipe_to_client[pipe_fd];
	Connection &c = _conns[client_fd];

	char buf[4096];
	ssize_t n = read(pipe_fd, buf, sizeof(buf));
	if (n > 0)
	{
		c.cgi.cgi_buf.append(buf, n);
		return false;
	}
	else if (n == 0)
	{

		return _finishCgi(indx, client_fd, _buildCgiResponse(c.cgi.cgi_buf, c));
	}
	else
	{
		c.request.setStatusCode(feedReturn::CGI_ERROR);
        HandlerResult result = responseHandler(c.request, _conf);
        return _finishCgi(indx, client_fd, result.response);
	}
}

bool Server::_finishCgi(size_t indx, int client_fd, const std::string &response)
{
    int pipe_fd = _pfds[indx].fd;
    Connection &c = _conns[client_fd];

    waitpid(c.cgi.pid, NULL, 0);
    c.out_buf = response;
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
    return true;
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

std::string  Server::_buildCgiResponse(const std::string &cgi_buf, Connection &c)
{
    std::string normalized = normalizeCgiOutput(cgi_buf);

    size_t separator = normalized.find("\r\n\r\n");
    if (separator == std::string::npos)
	{
		c.request.setStatusCode(feedReturn::CGI_ERROR);
		HandlerResult result = responseHandler(c.request, _conf);
		return result.response;
	}

    std::string cgi_headers = normalized.substr(0, separator);
    std::string body = normalized.substr(separator + 4);

    std::string status_line = "200 OK";
    std::string status_prefix = "Status: ";
    size_t status_pos = cgi_headers.find(status_prefix);
    if (status_pos != std::string::npos)
    {
        size_t status_end = cgi_headers.find("\r\n", status_pos);
        status_line = cgi_headers.substr(status_pos + status_prefix.size(),
                                         status_end - status_pos - status_prefix.size());
        cgi_headers.erase(status_pos, status_end - status_pos + 2);
    }

    return "HTTP/1.1 " + status_line + "\r\n"
        + cgi_headers + "\r\n"
        + "Content-Length: " + std::to_string(body.size()) + "\r\n"
        + "\r\n"
        + body;
}
