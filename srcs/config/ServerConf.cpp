#include "../../include/config/ServerConf.hpp"
#include <cstring> 


//set default value 
//host = 0.0.0.0 
//port = 80 by default
//server_name = ""
//client_max_body_size = 1m
ServerConf::ServerConf(): listen("0.0.0.0", 80),
	client_max_body_size(1024*1024),
	root("")
{
	server_name.push_back("");
	error_pages[400] = "/error_pages/400.html";
	error_pages[403] = "/error_pages/403.html";
	error_pages[404] = "/error_pages/404.html";
	error_pages[405] = "/error_pages/405.html";
	error_pages[413] = "/error_pages/413.html";
	error_pages[500] = "/error_pages/500.html";
	error_pages[502] = "/error_pages/502.html";
	
}

ServerConf &ServerConf::operator=(const ServerConf &other)
{
	if (this != &other)
	{
		listen = other.listen;
		server_name = other.server_name;
		client_max_body_size = other.client_max_body_size;
		error_pages = other.error_pages;
		locations = other.locations;
	}
	return *this;
}
void ServerConf::setListen(std::string &host, int port)
{
	if (!host.empty())
		this->listen.first = host;
	this->listen.second = port;
	convert_listen();
	return;
}

void	ServerConf::setRoot(std::string& root)
{
    this->root = root;
}

void ServerConf::convert_listen()
{
	if (!convert_ipv4(this->listen.first, this->listen_ip))
		throw std::runtime_error("Error: impossible ipv4 conversion.");
	this->listen_port = static_cast<uint16_t>(this->listen.second);
}

void ServerConf::setClientBodySize(size_t bodySize)
{
	this->client_max_body_size = bodySize;
}


void ServerConf::setServName(std::vector<std::string>& server_name)
{
    if (!server_name.empty())
	{
		for (std::string s : server_name)
			this->server_name.push_back(s);
	}
}


void ServerConf::setErrorPage(int error_code, std::string& error_path)
{
	this->error_pages[error_code] = error_path;
}

void ServerConf::setLocation(Location& loc)
{
	this->locations.push_back(loc);
}

const int& ServerConf::getPort() const
{
	return this->listen.second;
}

uint32_t ServerConf::getIpv4() const
{
	return this->listen_ip;
}
std::string ServerConf::getRoot() const
{
	return root;
}

size_t ServerConf::getClientSize() const
{
	return client_max_body_size;
}

const std::vector<std::string> &ServerConf::getServName() const
{
	return server_name;
}

const std::map<int, std::string> &ServerConf::getErrorPage() const
{
	return error_pages;
}

const std::vector<Location> &ServerConf::getLocation() const
{
	return locations;
}


void ServerConf::print() const
{
	std::cout << this->listen_ip << " " << this->listen_port << std::endl;
    std::cout << "server {" << std::endl;

    
    std::cout << "\tlisten ";
    if (!this->listen.first.empty())
        std::cout << this->listen.first << ":";
    std::cout << this->listen.second << ";" << std::endl;

	
    if (!this->server_name.empty())
    {
        std::cout << "\tserver_name";
        for (size_t i = 0; i < this->server_name.size(); i++)
            std::cout << " " << this->server_name[i];
        std::cout << ";" << std::endl;
    }

	std::cout << "\troot " << root + ";\n";
    std::cout << "\tclient_max_body_size " << this->client_max_body_size << ";" << std::endl;

    if (!this->error_pages.empty())
    {
        for (std::map<int, std::string>::const_iterator it = this->error_pages.begin();
             it != this->error_pages.end(); ++it)
            std::cout << "\terror_page " << it->first << " " << it->second << ";" << std::endl;
    }

    // locations
	std::cout << "\n" << std::endl;
    for (size_t i = 0; i < this->locations.size(); ++i)
    {
        std::cout << "\tlocation " << locations[i].getPath() << " {" << std::endl;
        locations[i].print();
        std::cout << "\t}" << std::endl;
    }

    std::cout << "}" << std::endl;
}

bool convert_ipv4(std::string &ip, uint32_t &ipv4)
{
    struct addrinfo hints, *res;

	std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(ip.c_str(), NULL, &hints, &res) != 0)
        return false;
    ipv4 = ntohl(((struct sockaddr_in*)res->ai_addr)->sin_addr.s_addr);
    freeaddrinfo(res);
    return true;
}
