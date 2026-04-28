#include "../include/io/Server.hpp"
#include "../include/handlers/Response.hpp"
#include "../include/config/WebservConf.hpp"


int testREQ(HTTPrequests& req)
{

	std::string raw = "DELETE /upload/test HTTP/1.1\r\n"
    "Host: localhost:8082\r\n"
    "Connection: keep-alive\r\n"
    "Content-Length: 0\r\n"
	"\r\n";

	feedReturn state;

	state = req.feed(raw);
	if (state != feedReturn::COMPLETE)
		std::cout << "ERROR" << std::endl;
	return 0;
}

int main(int argc, char **argv)
{
	setupSignals();
	ConfToken 	confFile;
	WebservConf	Webserv;
	if (argc > 2)
	{
		std::cout << "Error: invalid parameters\n";
		return 1;
	}

	try
	{
		if (argc == 2)
			confFile.setFile(argv[1]);
		else
			confFile.setFile("conf_files/default.conf");
	}
	catch(const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return 1;
	}

	error_conf status = setWebservConf(Webserv, confFile);
	if (!status.success)
	{
		std::cout << status.error_msg + "\n";
		return 1;
	}

	Server s;

	if(s.start(Webserv) != 0)
		return (1);
	s.run();
	return 0;
}

// Webserv.getAvailablePorts(); return a vector<int> with all port we have available to listen to
	//for now just vector[0] = 8080;

//GET
// curl -v http://localhost:8080/
//POST
// curl -v -X POST http://localhost:8080/upload -F "file=@/etc/hostname"
//DELETE
// curl -v -X DELETE http://localhost:8080/upload/hostname
//siege -c1 -t1M http://localhost:8080/cgi-bin/hello.py
//curl -X POST http://localhost:8080/upload -H "Content-Type:text/plain" \-d "agjahgsdjhagsdjasgdkjagsdgsahjgdahjgdshjagsd"
//test CGI POST
//curl -X POST http://localhost:8080/cgi-bin/upload.py -H "Content-Type:text/plain" \-d "megamega"
//printf 'POST /upload HTTP/1.1\r\nHost: 127.0.0.1:8080\r\nContent-Length: 9999\r\n\r\nshort' | nc -q 0 localhost 8080
