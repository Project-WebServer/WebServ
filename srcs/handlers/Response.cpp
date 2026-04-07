#include "../../include/handlers/Response.hpp"

Response::Response(): virtualServer(nullptr),
	_Location(nullptr),
	realPath(""),
	response(""),
	httpStatusCode(200)
{
}

void Response::setVirtualServ(const ServerConf* serv)
{
	this->virtualServer = serv;
}

void Response::setLocation(std::string uri)
{
	this->_Location = virtualServer->matchLocation(uri);
}

const ServerConf *Response::getVirtualServ() const
{
    return this->virtualServer;
}

const Location *Response::getLocation() const
{
	return this->_Location;
}

std::string Response::getRealPath() const
{
	return realPath;
}

const std::string Response::getResponse() const
{
    return this->response;
}

std::string Response::getIndexfile()
{
	std::vector<std::string> indexFiles = _Location->getIndex_files();

	if (indexFiles.size() == 0)
		return "";
	if (realPath.back() != '/')
		realPath = realPath + "/";
	for (size_t i = 0; i < indexFiles.size(); ++i)
	{
		std::string filePath = realPath;
		filePath += indexFiles[i];
		struct stat fileStat;
		if (stat(filePath.c_str(), &fileStat) == -1)
			continue;
		if (S_ISREG(fileStat.st_mode) && access(filePath.c_str(), R_OK) != -1)
			return filePath;
	}
	return "";
}

bool Response::hasCGI() const
{
	return _Location->hasCGI();
}

std::string Response::getCGIext() const
{
	return _Location->getCGIext();
}

std::string Response::getCgiInterpreter() const
{
	return _Location->getCgiInterpreter();
}

int Response::resolvePath(std::string uri)
{
	realPath = _Location->resolvePath(uri);

	if (realPath.find("..") != std::string::npos)
		return 403; // forbidden
	if (realPath.front() == '/')
		realPath = "." + realPath;
    return 200;
}
std::string Response::getHttpCode(int code)
{
	switch (code)
	{
		case 200: return " OK";                    // Request succeeded
		case 201: return " Created";				// Resource created successfully
		case 204: return " No Content";              // Resource deleted successfully
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

void Response::getDefErrorPage(std::string& body, int errorCode)
{
	std::map<int,std::string> DefErrorMap;
	ServerConf tmp;
	DefErrorMap = tmp.getDefaultErrorPage();
	if (DefErrorMap.size() != 0 && DefErrorMap.find(errorCode) != DefErrorMap.end())
	{
		std::string DefErrorPath = DefErrorMap.find(errorCode)->second;
		errmsg status = getFileContent(DefErrorPath, body);
		if (status.success)
			return;
	}
	return;
}

std::string Response::getErrorFileBody(int errorCode)
{
	std::map<int,std::string> errorMap;
	std::string body;

	if (virtualServer)
	{
		errorMap = virtualServer->getErrorPage();

		if (errorMap.size() != 0 && errorMap.find(errorCode) != errorMap.end())
		{
			std::string errorPath = "." +  virtualServer->getRoot() + errorMap.find(errorCode)->second; //need check '/'?

			errmsg status = getFileContent(errorPath, body);
			return body;
		}
		getDefErrorPage(body, errorCode);
		return body;
	}
	getDefErrorPage(body, errorCode);
	return body;
}

//commom use
errmsg Response::getFileContent(std::string& filePath, std::string& content)
{
	std::ifstream file(filePath);

	if(!file.is_open())
		return errmsg{false, std::strerror(errno)};

	std::ostringstream oss;
	oss << file.rdbuf();
	content = oss.str();
	return errmsg{true, ""};
}

std::string	Response::buildStatusLine(std::string httpVersion, int httpCode)
{
	std::ostringstream line;

	line << httpVersion << " " << httpCode << getHttpCode(httpCode) << "\r\n";
	return line.str();
}
// commom use
std::string Response::buildSuccessResponse(std::string path, int httpCode)
{
	std::ostringstream header;

	if (path.front() == '.')
		path.erase(0,1);
	header	<< buildStatusLine("HTTP/1.1", httpCode)
			<< "Location: " <<  path << "\r\n"
			<< "Content-Length: 0\r\n"
			<< "\r\n";
	httpStatusCode = httpCode;
	return header.str();
}

std::string Response::buildHeader(int httpCode, size_t bodySize, std::string contetType)
{
	std::ostringstream header;

	header	<< buildStatusLine("HTTP/1.1", httpCode)
			<< "Content-Type: " <<  contetType << "\r\n" // content type (text/html; imagem, etc)
			<< "Content-Length: " << bodySize << "\r\n"
			<< "Connection: keep-alive\r\n" //dynamic value?
			<< "\r\n";
	httpStatusCode = httpCode;
	return header.str();
}

void Response::handleHttpError(int errorCode)
{
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
}

bool	Response::isMethodAllowed(int Method)
{
	std::vector<httpMethod> methods = this->_Location->getAllowed_methods();

	std::cout << realPath << std::endl;
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
static int convert_host(std::string& ip, uint32_t& ipv4, int& port)
{
	std::string		host;

	if (ip.empty())
	{
		ipv4 = INADDR_ANY;
		port = 80;
		return 0;
	}
	size_t colonPos = ip.find(':');
	try
	{
		host = ip.substr(0, colonPos);
		port = stoi(ip.substr(colonPos + 1));
	}
	catch (std::exception &e)
	{
		return -1;
	}
	if (!convert_ipv4(host, ipv4))
		return -1;
	return 0;
}
static int		select_serv_n_location(HTTPrequests& request, WebservConf& servConf, Response& response)
{
	std::string host = request.getHeader().getValue("host");
	uint32_t ip;
	int port;
	if (convert_host(host, ip, port) == -1)
		return 500;
	const std::vector<ServerConf> *virtualServers= servConf.matchServer(ip, port);
	if (virtualServers == nullptr)
        return 500;
	const ServerConf &virtualServ = virtualServers->front();

	response.setVirtualServ(&virtualServ);
	response.setLocation(request.getPath());
	if (!response.isLocationValid())
	{
		std::cout << "Error: request`s uri not found.\n";
		return 404;
	}
	return 0;
}

//update also an error conde int request??
HandlerResult	responseHandler(HTTPrequests& request, WebservConf& servConf) //main function to handle respponse
{
	HandlerResult result;
	std::string uri;
	result.is_cgi = false;
	Response response;

	if (int i = select_serv_n_location(request, servConf, response); i != 0)
	{
		response.handleHttpError(i);
		result.response = response.getResponse();
		return result;
	}
	if (response.getLocation()->hasRedirection())
	{
		response.handleRedirect();
		result.response = response.getResponse();
        return result;
	}
	if (request.getMethods() == HTTPrequests::METHODS::POST)
	{
		uri = response.getLocation()->getUploadPath();
		if (uri == "")
			uri = request.getPath();
	}
	{
	if (int status = response.resolvePath(uri); status != 200)
		{
			response.handleHttpError(status);
			result.response = response.getResponse();
			return result;
		}
	}
	
	if(response.hasCGI())
	{
		result.is_cgi = true;
        result.cgi_path = response.getCgiInterpreter();
        result.cgi_script = response.getRealPath();
        return result;
	}

	switch (request.getMethods())
	{
	case HTTPrequests::METHODS::GET:
		response.handleGETrequest(request);
		break;
	case HTTPrequests::METHODS::POST:
		response.handlePOSTrequest(request);
		break;
	case HTTPrequests::METHODS::DELETE:
		response.handleDELETErequest(request);
		break;
	default:
		response.handleHttpError(405);
		break;
	}
	result.response = response.getResponse();
	return result;
}

void Response::handleGETrequest(HTTPrequests& request)
{
	struct stat fileStat;
	if (!isMethodAllowed((int)request.getMethods()))
		return handleHttpError(405);
	
	if (stat(realPath.c_str(), &fileStat) == -1)
		return handleHttpError(404);
	if (S_ISDIR(fileStat.st_mode))
	{
		if (access(realPath.c_str(), X_OK) == -1)
			return handleHttpError(403);
		std::string _realPath = getIndexfile();
		if (_realPath.empty())
		{
			if (_Location->getAutoindex())
			{
				std::string autoindexPage = buildAutoindex(realPath, request.getPath());
				response = buildHeader(200, autoindexPage.size(), "text/html");
				response += autoindexPage;
			}
			else
				return handleHttpError(403);
		}
		else
		{
			std::string body;
			if (!getFileContent(_realPath, body).success)
				return handleHttpError(403);
			response = buildHeader(200, body.size(), "text/html");
			response += body;
		}
	}
	else if(S_ISREG(fileStat.st_mode))
	{
		if (access(realPath.c_str(), R_OK) == -1)
			return handleHttpError(403);
		std::string body;
		if (!getFileContent(realPath, body).success)
			return handleHttpError(403);
		response = buildHeader(200, body.size(), "text/html");
		response += body;
	}
}

static std::string getBoundary(const std::string& contentType)
{
	std::string token = "boundary=";
	size_t start = contentType.find(token);
	if (start == std::string::npos)
		return "";
	return "--" + contentType.substr(start + token.size());
}

static std::string getFilename(const std::string& body, size_t& pos)
{
	std::string token = "filename=\"";
	size_t start = body.find(token, pos);
	if (start == std::string::npos)
		return "";
	start += token.size();
	size_t end = body.find('\"', start);
	if (end == std::string::npos)
		return "";
	pos = end + 1;
	return body.substr(start, end - start);
}

static std::string getContent(const std::string& body, const std::string& boundary, size_t& pos)
{
	size_t start = body.find("\r\n\r\n", pos);
	if (start == std::string::npos)
		return "";
	start += 4;
	std::string delimiter = boundary + "--";
	size_t end = body.find("\r\n" + boundary + "\r\n", start);
	if (end != std::string::npos)
	{
		pos = end + 1;
		return body.substr(start, end - start);
	}
	end = body.find("\r\n" + delimiter, start);
	if (end == std::string::npos)
		return "";
	pos = std::string::npos;
	return body.substr(start, end - start);
	
}

static int uploadFile(std::string& fileContent,std::string& uploadPath)
{
	std::ofstream file(uploadPath, std::ios::binary);
	if (!file.is_open())
   		return 500;
	file.write(fileContent.data(), fileContent.size());
	file.close();
	return 200;
}
void Response::handlePOSTrequest(HTTPrequests &request)
{
	if (!isMethodAllowed((int)request.getMethods()))
		return handleHttpError(405);
	if (request.getBody().size() > getVirtualServ()->getClientSize())
		return handleHttpError(413);
	std::string	boundary = getBoundary(request.getContType());
	if (boundary == "")
	{
		if (request.getContType().find("muiltpart/forma-data") == std::string::npos)
			return handleHttpError(415);
		return handleHttpError(400);
	}
	std::string uploadPath = getRealPath();
	struct stat fileStat;
	if (stat(uploadPath.c_str(), &fileStat) == -1)
		return handleHttpError(404);
	if (S_ISDIR(fileStat.st_mode))
	{
		if (uploadPath.back() != '/')
			uploadPath += "/";
	}
	size_t pos = 0;
	std::string body = request.getBody();
	while (pos != std::string::npos)
	{
		std::string fileName = getFilename(body, pos);
		if (fileName == "")
			return handleHttpError(400);
		std::string fileContent = getContent(body, boundary, pos);
		std::string path = uploadPath + fileName;
		if (int i = uploadFile(fileContent, path); i != 200)
			return handleHttpError(i);
	}
	response = buildSuccessResponse(uploadPath, 201);
	return;
}

std::string Response::buildAutoindex(const std::string &dirPath, const std::string &urlPath) const
{
    DIR* dir = opendir(dirPath.c_str());
    if (!dir)
        return "";

    std::string html;
    html += "<html><head><title>Index of " + urlPath + "</title></head><body>";
    html += "<h1>Index of " + urlPath + "</h1><hr><pre>";

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL)
    {
        std::string name = entry->d_name;
        if (name == ".")
            continue;

        std::string fullPath = dirPath + "/" + name;
        struct stat st;
        if (stat(fullPath.c_str(), &st) == -1)
            continue;

        bool isDir = S_ISDIR(st.st_mode);
        std::string displayName = isDir ? name + "/" : name;
        std::string link = urlPath + name + (isDir ? "/" : "");

        html += "<a href=\"" + link + "\">" + displayName + "</a>\n";
    }

    closedir(dir);
    html += "</pre><hr></body></html>";
    return html;
}

std::string Response::handleRedirect()
{
	std::string url = getLocation()->getRedirUrl();
	response = buildStatusLine("HTTP/1.1", 301)
		+ "Location: " + url + "\r\n"
		+ "Content-Length: 0\r\n\r\n";
	return getResponse();
}

static	int handleDeleteDir_n_file(std::string realPath, int flag)
{
	if (flag == 0)
	{
		DIR* dir = opendir(realPath.c_str());
		struct dirent* dirEntry;
		int content = 0;

		while ((dirEntry = readdir(dir)) != NULL)
		{
			if (strcmp(dirEntry->d_name, ".") == 0 || strcmp(dirEntry->d_name, "..") == 0)
				continue;
			content++;
		}
		closedir(dir);
		if (content != 0)
			return 409;
	}

	char absPath[PATH_MAX];
	if (realpath(realPath.c_str(), absPath) == NULL)
		return 404;
	char tmpPath[PATH_MAX];
	strncpy(tmpPath, absPath, PATH_MAX);
	std::string parentDir = dirname(tmpPath);
	if (access(parentDir.c_str(), W_OK) != 0)
		return 403;
	if (flag == 0)
	{
		if (rmdir(absPath) != 0)
			return 403;
	}
	else
	{
		if (unlink(absPath) != 0)
   			return 403;
	}
	return 0;
}

static std::string decodeUri(std::string& path)
{
	std::string decodPath;
	for (size_t i = 0; i < path.size(); ++i)
	{
		if (path.compare(i, 3, "%20") == 0)
		{
			decodPath += ' ';
			i += 2;
		}
		else
			decodPath += path[i];
	}
	return decodPath;
}
void Response::handleDELETErequest(HTTPrequests &request)
{
	if (!isMethodAllowed((int)request.getMethods()))
		return handleHttpError(405);
	struct stat fileStat;
	realPath = decodeUri(realPath);
	if (stat(realPath.c_str(), &fileStat) == -1)
		return handleHttpError(404);
	if(S_ISDIR(fileStat.st_mode))
	{
		if (int i = handleDeleteDir_n_file(realPath, 0); i != 0)
			return handleHttpError(i);
		response = buildStatusLine("HTTP/1.1", 204);
		return;
	}
	if (int i = handleDeleteDir_n_file(realPath, 1); i != 0)
		return handleHttpError(i);
	response = buildStatusLine("HTTP/1.1", 204) + "\r\n";
	return;
}
