
#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <sys/stat.h>
#include <fstream>
#include <sstream>
#include "../config/ServerConf.hpp"

class Response
{
	private:
		const ServerConf&	virtualServer;
		const Location&		Location;	

		std::string getHttpErrorBody(std::string& errorFile);
		std::string	getHttpCode(int code);
		std::string getErrorFilePath(int& errorCode);
	public:
		std::string handleHttpError(int errorCode);
		std::string buildHeader(int httpCode, size_t bodySize, std::string& contetType);


};

#endif
