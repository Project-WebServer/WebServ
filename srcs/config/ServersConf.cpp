#include "../../include/config/ServersConf.hpp"
#include "../../include/config/conf_parse.hpp"
#include "ServersConf.hpp"

//set default value 
//host = 0.0.0.0 
//port = 80 by default
//server_name = ""
//client_max_body_size = 1m
ServerConf::ServerConf(): listen("0.0.0.0", 80),
	client_max_body_size(1024*1024)
{
	server_name.push_back("");
	//set location 
	//set erro_pages location
}


void ServerConf::setListen(TokenLine &tokenLine)
{
	error_conf status;
	TokenLine tmp = tokenLine;

	status = isDirectiveValid(tokenLine);
	if (!status.success)
		throw std::runtime_error(status.error_msg);

	std::vector<std::string> &v = tokenLine.second;
	if (v.size() != 1)
		throw std::runtime_error("Error: invalid token near " + ConfToken::catTokens(tmp));
	std::string &ip = v[0];

	size_t colonPos = ip.find(':');
	try
	{
		if (colonPos == std::string::npos)
		{
			this->listen.second = stoi(ip.substr(colonPos + 1));
			if (listen.second < 0 || listen.second > 65535)
				throw std::runtime_error("Error: Invalid port value near " + ip);
		}
		else
		{
			std::string host = ip.substr(0, colonPos);
			if (!isHostValid(host).success)
				throw std::runtime_error("Error: Invalid IP address near " + ip);
			this->listen.first = host;
			this->listen.second = stoi(ip.substr(colonPos + 1));
			if (listen.second < 0 || listen.second > 65535)
				throw std::runtime_error("Error: Invalid port value near " + ip);
		}
	}
	catch (std::exception &e)
	{
		throw std::runtime_error("Error:: Invalid token near " + ConfToken::catTokens(tmp));
	}
	return;
}

void ServerConf::setClientSize(TokenLine &tokenLine)
{
	error_conf status;
	TokenLine tmp = tokenLine;

	status = isDirectiveValid(tokenLine);
	if (!status.success)
		throw std::runtime_error(status.error_msg);

	if (tokenLine.second.size() != 1)
		throw std::runtime_error("Error: invalid token near " + ConfToken::catTokens(tmp));

	std::string ip = tokenLine.second[0];
	size_t		size;
	try
	{
		size = std::stoi(ip);
	}
	catch (std::exception &e)
	{
		throw std::runtime_error("Error: invalid token: " + ip + " near " + ConfToken::catTokens(tmp));
	}

	char unit = std::tolower(ip.back());
	ip.pop_back();
	
	if (!isDigitOnly(ip))
		throw std::runtime_error("Error: invalid token: " + ip + " near " + ConfToken::catTokens(tmp));
	if (unit == 'k')
        this->client_max_body_size = size * 1024;
    else if (unit == 'm')
        this->client_max_body_size = size * 1024 * 1024;
    else if (unit == 'g')
        this->client_max_body_size =  size * 1024 * 1024 * 1024;
    else if(std::isdigit(unit))
        this->client_max_body_size = size;
	else
		throw std::runtime_error("Error: invalid client_max_body_size unit near " + ConfToken::catTokens(tmp));
}

//acept only a-z, A-Z, - and . format 
static bool isValidServerName(const std::string& name)
{
    if (name.empty() || name[0] == '-')
        return false;
    
    for (char c : name)
    {
        if (c != '-' && c != '.' && !::isalnum((unsigned char)c))
            return false;
    }
    return true;
}

void ServerConf::setServName(TokenLine &tokenLine)
{
    error_conf status;
    TokenLine tmp = tokenLine;

    status = isDirectiveValid(tokenLine);
    if (!status.success)
        throw std::runtime_error(status.error_msg);
    
    this->server_name.clear();
    
    for (const std::string& name : tokenLine.second)
    {
        if (!isValidServerName(name))
            throw std::runtime_error("Error: invalid server_name format near " + ConfToken::catTokens(tmp));
        
        this->server_name.push_back(name);
    }
}
