#include "../../include/config/conf_parse.hpp"


ConfToken::~ConfToken()
{
	confFile.close();
}
void ConfToken::setFile(std::string fileName)
{
	std::string _fileName = trim(fileName);
	if (_fileName == "")
		throw std::runtime_error("Error: invalid file name.");
	//check file extension (.conf or .cnf)
	confFile.open(_fileName);
	if (!confFile.is_open())
		throw std::runtime_error("Error: Could not open configuration file " + _fileName);
}

static tokenType _getTokenType(std::string token)
{
	if (token == "server") return tokenType::SERVER;
	if (token == "}") return tokenType::BLOCK_CLOSE;
	if (token == "location") return tokenType::LOCATION;
	if (token == "error_page") return tokenType::ERROR_PAGE;
	if (token == "listen") return tokenType::LISTEN;
	if (token == "client_max_body_size") return tokenType::MAX_CLIENT_SIZE;
	if (token == "root") return tokenType::ROOT;
	if (token == "allowed_methods") return tokenType::ALLOWED_METHODS;
	if (token == "index") return tokenType::INDEX;
	if (token == "autoindex") return tokenType::AUTO_INDEX;
	if (token == "server_name") return tokenType::SERVER_NAME;
	
	return tokenType::UNKNOWN;

}

std::string ConfToken::_convertTokenType(tokenType type)
{
	switch (type)
    {
        case tokenType::SERVER:          return "server";
        case tokenType::BLOCK_CLOSE:     return "}";
        case tokenType::LOCATION:        return "location";
        case tokenType::ERROR_PAGE:      return "error_page";
        case tokenType::LISTEN:            return "listen";
        case tokenType::MAX_CLIENT_SIZE: return "client_max_body_size";
        case tokenType::ROOT:            return "root";
        case tokenType::ALLOWED_METHODS: return "allowed_methods";
        case tokenType::INDEX:           return "index";
        case tokenType::AUTO_INDEX:      return "autoindex";
        case tokenType::SERVER_NAME:     return "server_name";
        case tokenType::UNKNOWN:         return "unknown";
		default: 						break;	
    }
	return "Undefined";
}

std::string ConfToken::catTokens(TokenLine& tokenLine)
{
	std::string s(ConfToken::_convertTokenType(tokenLine.first) + " ");
	std::vector<std::string> &v = tokenLine.second;
	for(size_t i = 0; i < v.size(); ++i)
	{
		s += v[i];
		if (i + 1 < v.size())
			s += " ";
	}
	return s;
}

void ConfToken::getNextLine(TokenLine& tokens)
{
	std::string					line;

	if (std::getline(confFile, line))
	{
		size_t commentPos = line.find('#');
		if (commentPos != std::string::npos) {
			line = line.substr(0, commentPos);
		}
		std::stringstream	ss(line);
		std::string			value;
		if (ss >> value)
		{
			tokens.first = _getTokenType(value);
			std::vector<std::string>	values;
			while(ss >> value)
				values.push_back(value);
			tokens.second = values;
		}
		else
			tokens.first = tokenType::NEWLINE;
		return;
	}
	tokens.first = tokenType::ENDOF;
	return;
}

TokenLine ConfToken::getNextToken()
{
	TokenLine	tokens;

	while(true)
	{
		getNextLine(tokens);
		if (tokens.first == tokenType::NEWLINE)
		{
			tokens.second.clear();
			continue;
		}
		else
			break;
	}
	return tokens;
}

void setListenServer(TokenLine &tokenLine, ServerConf& server)
{
	error_conf 		status;
	TokenLine 		tmp = tokenLine;
	std::string		host("");
	int				port;


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
			port = stoi(ip.substr(colonPos + 1));
			if (port < 0 || port > 65535)
				throw std::runtime_error("Error: Invalid port value near " + ip);
		}
		else
		{
			host = ip.substr(0, colonPos);
			if (!isHostValid(host).success)
				throw std::runtime_error("Error: Invalid IP address near " + ip);
			port = stoi(ip.substr(colonPos + 1));
			if (port < 0 || port > 65535)
				throw std::runtime_error("Error: Invalid port value near " + ip);
		}
	}
	catch (std::exception &e)
	{
		throw std::runtime_error("Error:: Invalid token near " + ConfToken::catTokens(tmp));
	}
	server.setListen(host, port);
	return;
}

void	setRootServer(TokenLine &tokenLine, ServerConf& server)
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
    server.setRoot(root);
}

void setClientSizeServer(TokenLine &tokenLine, ServerConf& server)
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
	size_t 		bodySize;
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
        bodySize = size * 1024;
    else if (unit == 'm')
        bodySize = size * 1024 * 1024;
    else if (unit == 'g')
        bodySize =  size * 1024 * 1024 * 1024;
    else if(std::isdigit(unit))
        bodySize = size;
	else
		throw std::runtime_error("Error: invalid client_max_body_size unit near " + ConfToken::catTokens(tmp));
	server.setClientBodySize(bodySize);
}

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

void setNameServer(TokenLine &tokenLine, ServerConf& server)
{
    error_conf status;
    TokenLine tmp = tokenLine;
	std::vector<std::string> server_name;

    status = isDirectiveValid(tokenLine);
    if (!status.success)
        throw std::runtime_error(status.error_msg);
    
    for (const std::string& name : tokenLine.second)
    {
        if (!isValidServerName(name))
            throw std::runtime_error("Error: invalid server_name format near " + ConfToken::catTokens(tmp));
        server_name.push_back(name);
    }
	server.setServName(server_name);
}

//handle just error code(4xx; 5xx) e path /error_pages/xxx format 
void setErrorPageServer(TokenLine &tokenLine, ServerConf& server)
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
	server.setErrorPage(error_code, error_path);
}

void setLocationServer(TokenLine &tokenLine, ConfToken& confile, ServerConf& server)
{
	Location loc;

	try
	{
		setPathLocation(tokenLine, loc);
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
				setRootLocation(loc_TokenLine, loc);
				break;
			case tokenType::ALLOWED_METHODS:
				setAllowed_methodsLocation(loc_TokenLine, loc);
				break;
			case tokenType::INDEX:
				setIndex_filesLocation(loc_TokenLine, loc);
				break;
			case tokenType::AUTO_INDEX:
				setAutoindexLocation(loc_TokenLine, loc);
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
	server.setLocation(loc);
}

//-----------Location validation--------//
//no implementantion of modifiers (=, ~, ~*, ^~, '')
void	setPathLocation(TokenLine &tokenLine, Location& loc)
{
    std::vector<std::string> &location = tokenLine.second;
	if (location.size() == 1 && location[0].size() > 1 && location[0].front() == '/' && location[0].back() == '{')
	{
		location[0] = "/";
		location.push_back("{");
	}
    if (location.size() != 2)
        throw std::runtime_error("Error: invalid location block near token " + ConfToken::catTokens(tokenLine));
    if (location[1] != "{")
        throw std::runtime_error("Error: missing open block '{' location near token " + ConfToken::catTokens(tokenLine));
    
    std::string &path = location[0];
    if (path.front() != '/')
        throw std::runtime_error("Error: invalid location path near token " + ConfToken::catTokens(tokenLine));
	
    loc.setPath(path);
}

void	setRootLocation(TokenLine &tokenLine, Location& loc)
{
    error_conf status;

	status = isDirectiveValid(tokenLine);
	if (!status.success)
		throw std::runtime_error(status.error_msg);
    std::vector<std::string> &token = tokenLine.second;
    if (token.size() != 1)
        throw std::runtime_error("Error: invalid location root directive near token " + ConfToken::catTokens(tokenLine));
    std::string &root = token[0];
    if (root.front() != '/')
        throw std::runtime_error("Error: invalid location root path near token " + ConfToken::catTokens(tokenLine));
    loc.setRoot(root);
}

void	setAllowed_methodsLocation(TokenLine &tokenLine, Location& loc)
{
    error_conf status;

	status = isDirectiveValid(tokenLine);
	if (!status.success)
		throw std::runtime_error(status.error_msg);
    std::vector<std::string> &token = tokenLine.second;
    if (token.size() < 1)
        throw std::runtime_error("Error: invalid allowed_method directive near token " + ConfToken::catTokens(tokenLine));
    for (auto method : token)
        loc.setAllowed_methods(method);
}

void	setIndex_filesLocation(TokenLine &tokenLine, Location& loc)
{
    error_conf status;

	status = isDirectiveValid(tokenLine);
	if (!status.success)
		throw std::runtime_error(status.error_msg);
    std::vector<std::string> &token = tokenLine.second;
    if (token.size() < 1)
        throw std::runtime_error("Error: invalid allowed_method directive near token " + ConfToken::catTokens(tokenLine));
    for (auto index : token)
        loc.setIndex_files(index);
}

void setAutoindexLocation(TokenLine &tokenLine, Location& loc)
{
	error_conf status;

	status = isDirectiveValid(tokenLine);
	if (!status.success)
		throw std::runtime_error(status.error_msg);
	std::vector<std::string> &token = tokenLine.second;
	if (token.size() != 1)
        throw std::runtime_error("Error: invalid directive near token " + ConfToken::catTokens(tokenLine));
	if (token[0] == "on")
		loc.setAutoindex_On();
}