
#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <cerrno>
#include <cstring>
#include <dirent.h>
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
		std::string         response;
		int					httpStatusCode;


		std::string	getHttpCode(int code);
		std::string getErrorFileBody(int errorCode);
		errmsg		getFileContent(std::string& filePath, std::string& content);
		std::string buildHeader(int httpCode, size_t bodySize, std::string contetType);

	public:

		Response();
		~Response(){};
		bool 		isLocationValid();
		bool		isMethodAllowed(int);
		
		
		void 		handleHttpError(int errorCode);
		int 		resolvePath(std::string uri);
		 
		// setters 
		void setVirtualServ(const ServerConf* serv);
		void setLocation(std::string uri);

		//getters 
		const ServerConf*	getVirtualServ() const;
		std::string			getRealPath() const;
		const std::string	getResponse() const;
		std::string			getIndexfile()const;

		void	handleGETrequest(HTTPrequests& request);
		std::string Response::buildAutoindex(const std::string& dirPath, const std::string& urlPath) const;


};

void	responseHandler(HTTPrequests& request, WebservConf& servConf);



#endif
