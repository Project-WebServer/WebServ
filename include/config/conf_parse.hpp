#ifndef CONF_PARSE_HPP
#define CONF_PARSE_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <utility>
#include <vector>

enum struct tokenType
{
	SERVER,
	BLOCK_CLOSE,
	LOCATION,
	ERROR_PAGE,
	HOST,
	PORT,
	MAX_CLIENT_SIZE,
	ROOT,
	ALLOWED_METHODS,
	INDEX,
	AUTO_INDEX,
	SERVER_NAME,
	NEWLINE,
	UNKNOWN,
	ENDOF
};

using TokenLine = typename std::pair<tokenType, std::vector<std::string>>;

class ConfToken
{
	private:
		std::fstream confFile;
		void getNextLine(TokenLine& tokens); //const;
	public:

		ConfToken(){};
		~ConfToken();

		void getFile(std::string fileName);
		TokenLine getNextToken(); //const
		std::string catTokens(TokenLine& tokenLine); //const
		std::string _convertTokenType(tokenType type); //const
};

std::string	trim(const std::string& s);
bool	isHostValid(std::vector<std::string> &v);

#endif