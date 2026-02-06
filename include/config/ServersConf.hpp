#ifndef SERVER_CONF_HPP
#define SERVER_CONF_HPP


#include <memory>
#include <map>
#include <utility>
#include "Location.hpp"


//store the servers 
class ServersConf
{
	private:
		std::string server_name;
		std::map<std::string, std::string> erro_pages;
		std::pair<std::string, int> listen; //host and port
		size_t client_max_body_size;
		std::map<std::string, Location> locations;


		public:
			ServersConf();
			~ServersConf();

			const std::string& get_serverName();
			const std::string& get_errorPage(std::string error_code);
			
};
 

#endif