#ifndef CONF_PARSE_HPP
#define CONF_PARSE_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <utility>
#include <vector>
#include <algorithm>
#include <cctype>

enum struct tokenType
{
	SERVER,
	BLOCK_CLOSE,
	LOCATION,
	ERROR_PAGE,
	MAX_CLIENT_SIZE,
	ROOT,
	LISTEN,
	ALLOWED_METHODS,
	INDEX,
	AUTO_INDEX,
	SERVER_NAME,
	NEWLINE,
	UNKNOWN,
	ENDOF
};

struct error_conf
{
	bool		success;
	std::string error_msg;
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

		static std::string _convertTokenType(tokenType type); //const
		static std::string catTokens(TokenLine& tokenLine); //const
};

std::string	trim(const std::string& s);
error_conf	isHostValid(std::string &s);
error_conf	isDirectiveValid(TokenLine& tokenLine);
bool		isDigitOnly(std::string &s);
// bool		isAlphaNum(std::string& s);

#endif