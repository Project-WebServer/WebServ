#ifndef CONNECTION_HPP
#define CONNECTION_HPP

# include <string>
# include <ctime>
// # include "src/http/request.hpp "

enum ParseState
{
	READING_REQUEST,//keep readinng
	REQUEST_COMPLETE,//parser said that request is ready
	SENDING_RESPONSE,//??
	CLOSING//close fd
};

struct CgiState
{
	int			pipe_read_fd;//fd from which read stdout CGI
	pid_t		pid;// for waitpid()
	std::string	cgi_buf;//collect CGI output
	time_t		started_at;//for CGI timeout
};

struct Connection
{
	std::string	in_buf;
	std::string	out_buf;
	ParseState	state;
	bool		keep_alive;// weather we nne dto keep connection after response
	time_t		last_activity;//unix timestamp (never hang indefinitely)
	//HTTPrequests request;
	// Трек C отримує request, повертає response.
   	// Трек A серіалізує response у out_buf.

   	//HttpResponse response; // можна зберігати тут або одразу серіалізувати
	bool		is_cgi;
	CgiState	cgi;
	Connection() : state(READING_REQUEST), keep_alive(false), last_activity(0), is_cgi(false) {}
	
	size_t bytesReceived() const
	{
	    return in_buf.size();
	}
};


#endif