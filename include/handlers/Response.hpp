
#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <sys/stat.h>
#include <fstream>
#include <sstream>
#include <cerrno>
#include <cstring>
#include "../config/ServerConf.hpp"

struct errmsg
{
	bool 	success;
	std::string msg;
};

class Response
{
	private:
		const ServerConf&	virtualServer;
		const Location*		Location;	

		std::string	getHttpCode(int code);
		std::string getErrorFileBody(int errorCode);
		errmsg		getFileContent(std::string& filePath, std::string& content);
		std::string buildHeader(int httpCode, size_t bodySize, std::string contetType);
	public:
		Response(const ServerConf&, std::string&);
		~Response(){};
		std::string handleHttpError(int errorCode);
		bool isLocationValid();


};

#endif
