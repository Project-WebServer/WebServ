#include "../../include/config/WebservConf.hpp"


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
				server.setListen(ServTokenLine);
				break;
			case tokenType::MAX_CLIENT_SIZE:
				server.setClientSize(ServTokenLine);
				break;
			case tokenType::SERVER_NAME:
				server.setServName(ServTokenLine);
				break;
			case tokenType::ERROR_PAGE:
				server.setErrorPage(ServTokenLine);
				break;
			case tokenType::LOCATION:
				server.setLocation(ServTokenLine, confFile);
				break;
			//unkhown case
			}
		}
		catch (std::exception &e)
		{
			return {false, e.what()};
		}
			
	}
	return {true, 0};
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
			webserv.pushServer(server);
		}
		else
		{
			std::cout << "UNKNOWN token type\n";
			exit(1); ///implementar retorno pois nao chama destructor locais
		}

	}
	return {true, 0};
}

int main()
{
	ConfToken 	confFile;
	WebservConf	Webserv;
	std::cout << "test1\n";
	try
	{
		confFile.getFile("exemple.conf");
	}
	catch(const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return 1;
	}
	std::cout << "test2\n";
	error_conf status = setWebservConf(Webserv, confFile);
	std::cout << "test3\n";
	Webserv.print();
	std::cout << "test4\n";
	
	return 0;
}