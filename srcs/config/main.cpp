#include "../../include/config/WebservConf.hpp"
#include "../../include/config/conf_parse.hpp"


static error_conf setServerConf(ServerConf& server, ConfToken& confFile, TokenLine& tokenLine)
{
	if (tokenLine.second.size() != 1 || tokenLine.second[0] != "{")
		return {false, "Erro: Invalid syntax in configuration file " + confFile.catTokens(tokenLine)};

	TokenLine  ServTokenLine;
	while (true)
	{
		ServTokenLine = confFile.getNextToken();
		
		if (ServTokenLine.first == tokenType::ENDOF)
			return {false, "Erro: Missing '}' close directive block (server)"};
		if (ServTokenLine.first == tokenType::BLOCK_CLOSE)
			break;
		try
		{
			switch (ServTokenLine.first)
			{
			case tokenType::LISTEN:
				setListenServer(ServTokenLine, server);
				break;
			case tokenType::ROOT:
				setRootServer(ServTokenLine, server);
				break;
			case tokenType::MAX_CLIENT_SIZE:
				setClientSizeServer(ServTokenLine, server);
				break;
			case tokenType::SERVER_NAME:
				setNameServer(ServTokenLine, server);
				break;
			case tokenType::ERROR_PAGE:
				setErrorPageServer(ServTokenLine, server);
				break;
			case tokenType::LOCATION:
				setLocationServer(ServTokenLine, confFile, server);
				break;
			default:
				continue; //handle it better 
			}
		}
		catch (std::exception &e)
		{
			return {false, e.what()};
		}
			
	}
	return {true, "Success"};
}
	

static error_conf setWebservConf(WebservConf &webserv, ConfToken& confFile)
{
	
	while (true)
	{
		TokenLine	tokenLine = confFile.getNextToken();
		if (tokenLine.first == tokenType::ENDOF)
			break;
		if (tokenLine.first == tokenType::SERVER)
		{
			ServerConf server;
			error_conf status;
			//implement the function below into Webserv class
			status = setServerConf(server, confFile, tokenLine);
			if (!status.success)
				return status;
			ENDPOINT endPoint{server.getIpv4(), server.getPort()};
			webserv.pushServer(server, endPoint);
		}
		else
		{
			std::cout << "UNKNOWN token type\n";
			exit(1); ///implementar retorno pois nao chama destructor locais
		}

	}
	return {true, "Success"};
}

int main()
{
	ConfToken 	confFile;
	WebservConf	Webserv;
	try
	{
		confFile.setFile("exemple.conf");
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
	std::cout << "\nNUmber of servers: " << Webserv.getNumberOfServers() << "\n";
	
	return 0;
}
