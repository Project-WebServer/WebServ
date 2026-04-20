#ifndef CONNECTION_HPP
#define CONNECTION_HPP

# include "../http/request.hpp"
# include <string>
# include <ctime>

enum ParseState
{
	READING_REQUEST,
	REQUEST_COMPLETE,
	SENDING_RESPONSE,
	CLOSING,
	CGI_READING
};

struct CgiState
{
	pid_t			pid;
	int				pipe_read_fd;
	time_t			started_at;
	std::string		cgi_buf;
	std::string 	cgi_path;
	std::string 	cgi_script;
};

struct Connection
{
	std::string		in_buf;
	std::string		out_buf;
	ParseState		state;
	bool			keep_alive;
	time_t			last_activity;
	HTTPrequests	request;
	bool			is_cgi;
	CgiState		cgi;
	Connection() : state(READING_REQUEST), keep_alive(false), last_activity(0), is_cgi(false)
	{
		cgi.pid = -1;
		cgi.pipe_read_fd = -1;
		cgi.started_at = 0;
	}
	
	size_t bytesReceived() const
	{
	    return in_buf.size();
	}
};


#endif