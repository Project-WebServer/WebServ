#include "../../include/handlers/Response.hpp"
#include "Response.hpp"


Response::Response()
{
	this->virtualServer = nullptr;
	this->_Location = nullptr;
}

void Response::setVirtualServ(const ServerConf* serv)
{
	this->virtualServer = serv;
}

void Response::setLocation(std::string& uri)
{
	this->_Location = virtualServer->matchLocation(uri);
}

const ServerConf *Response::getVirtualServ() const
{
    return this->virtualServer;
}
std::string Response::getHttpCode(int code)
{
	switch (code)
	{
		case 200: return " OK";                    // Request succeeded
		case 201: return " Created";               // Resource created successfully
		case 301: return " Moved Permanently";     // Resource moved to a new URL
		case 400: return " Bad Request";           // Malformed or invalid request
		case 403: return " Forbidden";             // No permission to access the resource
		case 404: return " Not Found";             // Resource does not exist
		case 405: return " Method Not Allowed";    // HTTP method not supported for this route
		case 408: return " Request Timeout";       // Client took too long to send the request
		case 409: return " Conflict";              // Request conflicts with current resource state
		case 413: return " Payload Too Large";     // Body exceeds client_max_body_size
		case 415: return " Unsupported Media Type"; // Content-Type not supported
		case 500: return " Internal Server Error"; // Unexpected server-side failure
		case 502: return " Bad Gateway";           // CGI returned an invalid response
		case 504: return " Gateway Timeout";       // CGI took too long to respond
	}
	return "";
}

std::string Response::getErrorFileBody(int errorCode)
{
	std::map<int,std::string> errorMap = virtualServer->getErrorPage();

	if (errorMap.size() != 0 && errorMap.find(errorCode) != errorMap.end())
	{
		std::string errorPath = "." +  virtualServer->getRoot() + errorMap.find(errorCode)->second; //need check '/'?
		std::string body;

		errmsg status = getFileContent(errorPath, body);
		if (status.success)
			return body;
	}

	std::map<int,std::string> DefErrorMap = virtualServer->getDefaultErrorPage();
	if (DefErrorMap.size() != 0 && DefErrorMap.find(errorCode) != DefErrorMap.end())
	{
		std::string DefErrorPath = DefErrorMap.find(errorCode)->second;
		std::string body;

		errmsg status = getFileContent(DefErrorPath, body);
		if (status.success)
			return body;
	}
	return "";
}

//commom use 
errmsg Response::getFileContent(std::string& filePath, std::string& content)
{
	std::ifstream file(filePath);
	
	if(!file.is_open())
	{
		return errmsg{false, std::strerror(errno)};
	}

	std::ostringstream oss;

	oss << file.rdbuf();
	content = oss.str();
	return errmsg{true, ""};
}

//commom use 
std::string Response::buildHeader(int httpCode, size_t bodySize, std::string contetType)
{
	std::ostringstream header;

	header	<< "HTTP/1.1 " << httpCode << getHttpCode(httpCode) << "\r\n"
			<< "Content-Type: " <<  contetType << "\r\n" // content type (text/html; imagem, etc)
			<< "Content-Length: " << bodySize << "\r\n"
			<< "Connection: keep-alive\r\n" //dynamic value??
			<< "\r\n";

	return header.str();
}

std::string Response::handleHttpError(int errorCode)
{
	std::string response;
	std::string body = getErrorFileBody(errorCode);
	if (body.empty())
	{
		errorCode = 500;
		body = getErrorFileBody(errorCode);
		if (body.empty())
			body = "<html><body><h1>500 Internal Server Error</h1></body></html>";
	}
	response = buildHeader(errorCode, body.size(), "text/html");
	response += body;
	return response;
}

bool	Response::isMethodAllowed(int Method)
{
	std::vector<httpMethod> methods = this->_Location->getAllowed_methods();

	if (methods.size() == 0)
	{
		std::cout << "Error: method not allowed.\n";
		return false;
	}
	for (size_t i = 0; i < methods.size(); ++i)
	{
		if (Method == (int)methods[i])
			return true;
	}
	std::cout << "Error: method not allowed.\n";
	return false;
}

bool Response::isLocationValid()
{
	if (!_Location) 
		return false;
	return true;
}

// turn into methods ----------------------------

errmsg		select_serv_n_location(HTTPrequests& request, WebservConf& servConf, Response& response)
{
	//get from HTTPrequests the (uint32_t)ip and (int)port
	//temporaly
	uint32_t ip = servConf.getAvailableEndPoints().front().ip;
	int port = servConf.getAvailableEndPoints().front().port;
	const std::vector<ServerConf> *virtualServers= servConf.matchServer(ip, port);
	const ServerConf &virtualServ = virtualServers->front();

	response.setVirtualServ(&virtualServ);
	response.setLocation(request.getPath()); // ask Yuleum to implemnt getters
	if (!response.isLocationValid())
	{
		std::cout << "Error: request`s uri not found.\n";
		return {false, ""};
	}
	return {true, ""};
}


std::string	responseHandler(HTTPrequests& request, WebservConf& servConf) //main function to handle respponse
{
	Response response;

	if (!select_serv_n_location(request, servConf, response).success)
		return response.handleHttpError(404);
	if (!response.isMethodAllowed((int)request.getMethod()))
		return response.handleHttpError(405);
	if (request.getBody().size() > response.getVirtualServ()->getClientSize()) // create getter for body size in HTTPrequests
		return response.handleHttpError(413);
	// if (request.getMethod() == HTTPrequests::METHODS::POST && request.getHeader().getValue("Content-Type").empty())
	// 	return response.handleHttpError(400); // check if content type is valid for POST method, create getter for header in HTTPrequests
	if (int status = )
	
}