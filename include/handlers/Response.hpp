
#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <cerrno>
#include <cstring>
#include "../config/WebservConf.hpp"
#include "../http/request.hpp"

struct errmsg
{
	bool 	success;
	std::string msg;
};

class Response
{
	private:
		const ServerConf*	virtualServer; 
		const Location*		_Location;
		std::string			realPath;

		std::string	getHttpCode(int code);
		std::string getErrorFileBody(int errorCode);
		errmsg		getFileContent(std::string& filePath, std::string& content);
		std::string buildHeader(int httpCode, size_t bodySize, std::string contetType);
	public:

		Response();
		~Response(){};
		std::string handleHttpError(int errorCode);
		bool 		isLocationValid();
		bool		isMethodAllowed(int);

		void setVirtualServ(const ServerConf* serv);
		void setLocation(std::string& uri);

		const ServerConf* getVirtualServ() const;
		int Response::resolvePath(std::string& uri);

};

std::string	responseHandler(HTTPrequests& request, WebservConf& servConf);



#endif
