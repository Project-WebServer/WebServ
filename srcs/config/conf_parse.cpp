#include "../../include/config/conf_parse.hpp"


ConfToken::~ConfToken()
{
	confFile.close();
}
void ConfToken::getFile(std::string fileName)
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
	if (token == "host") return tokenType::HOST;
	if (token == "listen") return tokenType::PORT;
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
        case tokenType::HOST:            return "host";
        case tokenType::PORT:            return "listen";
        case tokenType::MAX_CLIENT_SIZE: return "client_max_body_size";
        case tokenType::ROOT:            return "root";
        case tokenType::ALLOWED_METHODS: return "allowed_methods";
        case tokenType::INDEX:           return "index";
        case tokenType::AUTO_INDEX:      return "autoindex";
        case tokenType::SERVER_NAME:     return "server_name";
        case tokenType::UNKNOWN:         return "unknown";
    }
}

std::string ConfToken::catTokens(TokenLine& tokenLine)
{
	std::string s(_convertTokenType(tokenLine.first) + " ");
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
}
