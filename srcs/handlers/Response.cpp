#include "../../include/handlers/Response.hpp"
#include "Response.hpp"

std::string Response::getHttpErrorBody(std::string& errorFile)
{
	return std::string();
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
}

std::string Response::getErrorFilePath(int &errorCode)
{
	std::map<int,std::string> error = virtualServer.getErrorPage();

	if (error.size() != 0 && error.find(errorCode) != error.end())
	{

	}
	else
	{
		
	}

	error 
	return std::string();
}

std::string Response::handleHttpError(int errorCode)
{
	return std::string();
}


std::string Response::buildHeader(int httpCode, size_t bodySize, std::string& contetType)
{
	std::ostringstream header;

	header	<< "HTTP/1.1 " << httpCode << getHttpCode(httpCode) << "\r\n"
			<< "Content-Type: " <<  contetType << "\r\n"
			<< "Content-Length: " << bodySize << "\r\n"
			<< "Connection: keep-alive\r\n" //dynamic value??
			<< "\r\n";

	return header.str();
}
