
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
		std::string	buildStatusLine(std::string httpVersion, int httpCode);
		std::string buildSuccessResponse(std::string httpVersion, int httpCode);

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
		const Location*		getLocation() const;
		std::string			getRealPath() const;
		const std::string	getResponse() const;
		std::string			getIndexfile();

		void		handleGETrequest(HTTPrequests& request);
		void		handlePOSTrequest(HTTPrequests& request);
		std::string	buildAutoindex(const std::string& dirPath, const std::string& urlPath) const;
		std::string	handleRedirect(); 


};

void	responseHandler(HTTPrequests& request, WebservConf& servConf, std::string& _response);



#endif
