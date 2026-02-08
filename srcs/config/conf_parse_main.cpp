#include "../../include/config/WebservConf.hpp"
#include "../../include/config/conf_parse.hpp"

/// funcao para criar um objeto webserve
//loop para criar multiplos objetos server para webserv
//loop para criar multiplos objetos location para um server

//TokenLine getNextToken();

ServerConf setServerConf(ConfToken& confFile, TokenLine& tokenLine)
{
	if (tokenLine.second.size() != 1 || tokenLine.second[0] != "{")
		throw std::runtime_error("Erro: Invalid syntax in configuration file ");

	ServerConf server;
	TokenLine  ServTokenLine;

	while (true)
	{
		ServTokenLine = confFile.getNextToken();
		
		if (ServTokenLine.first == tokenType::ENDOF)
			//create an error classe or struc
		if (ServTokenLine.first == tokenType::BLOCK_CLOSE)
			break;
		switch (ServTokenLine.first)
		{
		case tokenType::HOST:
			server.setHost(ServTokenLine.second);
		case tokenType::PORT:
			server.setPort(ServTokenLine.second);
		case tokenType::MAX_CLIENT_SIZE:
			server.setClientSize(ServTokenLine.second);
		case tokenType::SERVER_NAME:
			server.setServName(ServTokenLine.second);
		case tokenType::ERROR_PAGE:
			server.setErrorPage(ServTokenLine.second);
		case tokenType::LOCATION:
			server.setLocation(ServTokenLine);
		
			
		}
	}
	
	
}

WebservConf setWebservConf(std::string conf_file) //return an erro than Webserv
{
	WebservConf webserv;
	ConfToken	confFile;

	try
	{
		confFile.getFile(conf_file);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		exit(1);
	}

	while (true)
	{
		TokenLine	tokenLine = confFile.getNextToken();
		if (tokenLine.first == tokenType::ENDOF)
			break;
		if (tokenLine.first == tokenType::SERVER)
		{
			try
			{
				webserv.pushServer(setServerConf(confFile, tokenLine));
			}
			catch (std::runtime_error &e)
			{
				std::cout << e.what() + confFile.catTokens(tokenLine);
			}
		}
		else
		{
			std::cout << "UNKNOWN token type\n";
			exit(1); ///implementar retorno pois nao chama destructor locais
		}

	}


	
	
}



int main(int argc, char** argv)
{

}