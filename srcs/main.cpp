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

	
    // std::string body =  "Content-Type: multipart/form-data; boundary=----WebKitFormBoundaryoJIXq9bnpRUnLLP4\r\n"
    // "------WebKitFormBoundaryoJIXq9bnpRUnLLP4\r\n"
    // "Content-Disposition: form-data; name=\"image\"; filename=\"test\"\r\n"
    // "Content-Type: image/jpeg\r\n"
    // "\r\n"
    // "fake image content\r\n"
    // "------WebKitFormBoundaryoJIXq9bnpRUnLLP4--";
	// raw += body;

	feedReturn state;

	state = req.feed(raw);
	// std::string body = req.getBody();
	// std::cout << body << std::endl;
	if (state != feedReturn::COMPLETE)
		std::cout << "ERROR" << std::endl;
	// else
	// {
	// 	req.printRequest();
	// 	std::cout << std::endl;
	// 	req.printHeader();
	// 	std::cout << std::endl;
	// 	req.printBody();
	// }
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
	// Webserv.print();
	// const std::vector<ServerConf>* ptr = nullptr;
	// ptr = Webserv.matchServer(0, 8080);
	// if (ptr != nullptr)
	// {
	// 	std::cout << ptr->size() << "\n"; 
	// 	ptr->front().print();
	// }

	// std::string uri = "/";
	// HTTPrequests testRequest;
	// testREQ(testRequest);
	// std::string response;
	// responseHandler(testRequest, Webserv, response);
	// std::cout << response << std::endl;

//---------------------------------------------------------------------//
// 	const std::vector<ENDPOINT> &endpoints = Webserv.getAvailableEndPoints();
//     std::cout << "endpoints count: " << endpoints.size() << std::endl;
//     for (size_t i = 0; i < endpoints.size(); i++)
// 		std::cout << "  ip=" << endpoints[i].ip << " port=" << endpoints[i].port << std::endl;
// //---------------------------------------------------------------------//

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