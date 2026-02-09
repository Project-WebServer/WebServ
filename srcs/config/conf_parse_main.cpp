#include "../../include/config/WebservConf.hpp"
#include "../../include/config/conf_parse.hpp"

/// funcao para criar um objeto webserve
//loop para criar multiplos objetos server para webserv
//loop para criar multiplos objetos location para um server

//TokenLine getNextToken();

error_conf setServerConf(ServerConf& server, ConfToken& confFile, TokenLine& tokenLine)
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
			case tokenType::MAX_CLIENT_SIZE:
				server.setClientSize(ServTokenLine);
			case tokenType::SERVER_NAME:
				server.setServName(ServTokenLine);
			case tokenType::ERROR_PAGE:
				server.setErrorPage(ServTokenLine);
			case tokenType::LOCATION:
				server.setLocation(ServTokenLine);
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
	

error_conf setWebservConf(WebservConf &webserv, std::string conf_file) //return an erro than Webserv
{
	ConfToken	confFile;

	try
	{
		confFile.getFile(conf_file);
	}
	catch(const std::exception& e)
	{
		return {false, e.what()};
	}

	while (true)
	{
		TokenLine	tokenLine = confFile.getNextToken();
		if (tokenLine.first == tokenType::ENDOF)
			break;
		if (tokenLine.first == tokenType::SERVER)
		{
			ServerConf server;
			error_conf status;

			status = setServerConf(server, confFile, tokenLine);
			if (!status.success)
				return status;
		
		}
		else
		{
			std::cout << "UNKNOWN token type\n";
			exit(1); ///implementar retorno pois nao chama destructor locais
		}

	}
	return {true, 0};
}

