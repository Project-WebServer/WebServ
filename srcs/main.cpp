#include "../include/io/Server.hpp"
#include "../include/config/WebservConf.hpp"

int main(int argc, char **argv)
{
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
	// const std::vector<ServerConf>* ptr = nullptr;
	// ptr = Webserv.matchServer(0, 8080);
	// if (ptr != nullptr)
	// {
	// 	std::cout << ptr->size() << "\n"; 
	// 	ptr->front().print();
	// }

	std::cout << "Available port: " << Webserv.getAvailablePorts().front() << std::endl;

	Server s;

	if(s.start() != 0)
		return (1);
	s.run();
	return 0;
}

// Webserv.getAvailablePorts(); return a vector<int> with all port we have available to listen to
	//for now just vector[0] = 8080;