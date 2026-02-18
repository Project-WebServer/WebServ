#include "../../include/config/ServerConf.hpp"
#include "../../include/config/conf_parse.hpp"



//set default value 
//host = 0.0.0.0 
//port = 80 by default
//server_name = ""
//client_max_body_size = 1m
ServerConf::ServerConf(): listen("0.0.0.0", 80),
	client_max_body_size(1024*1024),
	root("www/html")
{
	server_name.push_back("");

	//set default erro_pages location 
}

ServerConf &ServerConf::operator=(const ServerConf &other)
{
	if (this != &other)
	{
		listen = other.listen;
		server_name = other.server_name;
		client_max_body_size = other.client_max_body_size;
		erro_pages = other.erro_pages;
		locations = other.locations;
	}
	return *this;
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

void	ServerConf::setRoot(TokenLine &tokenLine)
{
    error_conf status;

	status = isDirectiveValid(tokenLine);
	if (!status.success)
		throw std::runtime_error(status.error_msg);
    std::vector<std::string> &token = tokenLine.second;
    if (token.size() != 1)
        throw std::runtime_error("Error: invalid server root directive near token " + ConfToken::catTokens(tokenLine));
    std::string &root = token[0];
    if (root.front() != '/')
        throw std::runtime_error("Error: invalid server root path near token " + ConfToken::catTokens(tokenLine));
    this->root = root;
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

//handle just error code(4xx; 5xx) e path /error_pages/xxx format 
void ServerConf::setErrorPage(TokenLine &tokenLine)
{
	error_conf	status;

	status = isDirectiveValid(tokenLine);
	if (!status.success)
		throw std::runtime_error(status.error_msg);
	if (tokenLine.second.size() != 2)
		throw std::runtime_error("Error: invalid token near " + ConfToken::catTokens(tokenLine));
	
	std::string	&error_code_str = tokenLine.second[0];
	std::string	&error_path = tokenLine.second[1];
	int	error_code;
	try
	{
		size_t pos;
		error_code = std::stoi(error_code_str, &pos);
		if (pos < error_code_str.size())
			throw std::runtime_error("Error: invalid error code near token " + ConfToken::catTokens(tokenLine));
	}
	catch (std::exception &e)
	{
		throw std::runtime_error(e.what());
	}

	if (error_path.front() != '/')
		throw std::runtime_error("Error: invalid error code path near token " + ConfToken::catTokens(tokenLine));
	if (error_code < 400 || error_code > 599)
		throw std::runtime_error("Error: invalid error code value near token " + ConfToken::catTokens(tokenLine));
	this->erro_pages[error_code] = error_path;
}

void ServerConf::setLocation(TokenLine &tokenLine, ConfToken& confile)
{
	Location loc;

	try
	{
		loc.setPath(tokenLine);
	}
	catch (std::exception &e)
	{
		throw std::runtime_error(e.what());
	}

	TokenLine loc_TokenLine;
	while (true)
	{
		loc_TokenLine = confile.getNextToken();
		if (loc_TokenLine.first == tokenType::ENDOF)
			throw std::runtime_error("Erro: Missing '}' close directive block (location) near " + ConfToken::catTokens(loc_TokenLine));
		if (loc_TokenLine.first == tokenType::BLOCK_CLOSE)
			break;
		try
		{
			switch (loc_TokenLine.first)
			{
			case tokenType::ROOT:
				loc.setRoot(loc_TokenLine);
				break;
			case tokenType::ALLOWED_METHODS:
				loc.setAllowed_methods(loc_TokenLine);
				break;
			case tokenType::INDEX:
				loc.setIndex_files(loc_TokenLine);
				break;
			case tokenType::AUTO_INDEX:
				loc.setAutoindex(loc_TokenLine);
				break;
			default:
				continue; //handle it better 
			//unkhown case and more cases
			}
		}
		catch (std::exception &e)
		{
			throw std::runtime_error(e.what());
		}
	}
	this->locations[loc.getPath()] = loc;
}

const std::pair<std::string, int>& ServerConf::getListen() const
{
	return listen;
}

std::string ServerConf::getRoot() const
{
	return root;
}

size_t ServerConf::getClientSize() const
{
	return client_max_body_size;
}

const std::vector<std::string> &ServerConf::getServName() const
{
	return server_name;
}

const std::map<int, std::string> &ServerConf::getErrorPage() const
{
	return erro_pages;
}

const std::map<std::string, Location> &ServerConf::getLocation() const
{
	return locations;
}


void ServerConf::print() const
{
    std::cout << "server {" << std::endl;

    
    std::cout << "\tlisten ";
    if (!this->listen.first.empty())
        std::cout << this->listen.first << ":";
    std::cout << this->listen.second << ";" << std::endl;

    
    if (!this->server_name.empty())
    {
        std::cout << "\tserver_name";
        for (size_t i = 0; i < this->server_name.size(); i++)
            std::cout << " " << this->server_name[i];
        std::cout << ";" << std::endl;
    }

    
    std::cout << "\tclient_max_body_size " << this->client_max_body_size << ";" << std::endl;

    if (!this->erro_pages.empty())
    {
        for (std::map<int, std::string>::const_iterator it = this->erro_pages.begin();
             it != this->erro_pages.end(); ++it)
            std::cout << "\terror_page " << it->first << " " << it->second << ";" << std::endl;
    }

    // locations
	std::cout << "\n" << std::endl;
    for (std::map<std::string, Location>::const_iterator it = this->locations.begin();
         it != this->locations.end(); ++it)
    {
        std::cout << "\tlocation " << it->first << " {" << std::endl;
        it->second.print();
        std::cout << "\t}" << std::endl;
    }

    std::cout << "}" << std::endl;
}

