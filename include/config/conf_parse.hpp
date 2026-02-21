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
#include "ServerConf.hpp"

class WebservConf;

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

enum struct httpMethod
{
	GET,
	POST,
	DELETE,
	UNKNOWN
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

		void 		setFile(std::string fileName);
		TokenLine	getNextToken(); //const

		static std::string _convertTokenType(tokenType type); //const
		static std::string catTokens(TokenLine& tokenLine); //const
};

std::string		trim(const std::string& s);
error_conf		isHostValid(std::string &s);
error_conf		isDirectiveValid(TokenLine& tokenLine);
bool			isDigitOnly(std::string &s);
httpMethod		getHttpMethod(std::string token);
void			setRootServer(TokenLine &tokenLine, ServerConf& server);
void 			setListenServer(TokenLine &tokenLine, ServerConf& server);
void 			setClientSizeServer(TokenLine &tokenLine, ServerConf& server);
void 			setNameServer(TokenLine &tokenLine, ServerConf& server);
void 			setErrorPageServer(TokenLine &tokenLine, ServerConf& server);
void			setLocationServer(TokenLine &tokenLine, ConfToken& confile, ServerConf& server);
void			setPathLocation(TokenLine &tokenLine, Location& loc);
void			setRootLocation(TokenLine &tokenLine, Location& loc);
void			setAllowed_methodsLocation(TokenLine &tokenLine, Location& loc);
void			setIndex_filesLocation(TokenLine &tokenLine, Location& loc);
void 			setAutoindexLocation(TokenLine &tokenLine, Location& loc);
error_conf 		setServerConf(ServerConf& server, ConfToken& confFile, TokenLine& tokenLine);
error_conf 		setWebservConf(WebservConf &webserv, ConfToken& confFile);


#endif