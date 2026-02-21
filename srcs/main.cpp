#include "../include/io/socket.hpp"
#include "../include/config/WebservConf.hpp"

int main()
{
	ConfToken 	confFile;
	WebservConf	Webserv;
	try
	{
		confFile.setFile("srcs/exemple.conf");
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
	Webserv.print();
	
	Server s;

	if(s.start() != 0)
		return (1);
	s.run();
	return 0;
}