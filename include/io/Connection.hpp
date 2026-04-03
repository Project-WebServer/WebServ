#ifndef CONNECTION_HPP
#define CONNECTION_HPP

# include "../http/request.hpp"
# include <string>
# include <ctime>

enum ParseState
{
	READING_REQUEST,//keep readinng
	REQUEST_COMPLETE,//parser said that request is ready
	SENDING_RESPONSE,//??
	CLOSING,//close fd
	CGI_READING
};

struct CgiState
{
	pid_t		pid;// for waitpid()
	int			pipe_read_fd;//fd from which read stdout CGI
	time_t		started_at;//for CGI timeout
	std::string	cgi_buf;//collect CGI output
	std::string cgi_path;
	std::string cgi_script;
};

struct Connection
{
	std::string	in_buf;
	std::string	out_buf;
	ParseState	state;
	bool		keep_alive;// weather we nne dto keep connection after response
	time_t		last_activity;//unix timestamp (never hang indefinitely)
	HTTPrequests request;
	bool		is_cgi;
	CgiState	cgi;
	Connection() : state(READING_REQUEST), keep_alive(false), last_activity(0), is_cgi(false) {}
	
	size_t bytesReceived() const
	{
	    return in_buf.size();
	}
};


#endif