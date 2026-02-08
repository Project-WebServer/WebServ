#ifndef SERVER_CONF_HPP
#define SERVER_CONF_HPP


#include <memory>
#include <map>
#include <utility>
#include "Location.hpp"


//store the servers 
class ServerConf
{
	private:
		std::string							server_name;
		std::map<std::string, std::string>	erro_pages;
		std::string							host;
		unsigned int						port;
		size_t								client_max_body_size;
		std::map<std::string, Location> 	locations;


		public:
			ServerConf();
			~ServerConf();

			const std::string& get_serverName();
			const std::string& get_errorPage(std::string error_code);
			
			void setHost(std::vector<std::string> &v);
			void setPort(std::vector<std::string> &v);
			void setClientSize(std::vector<std::string> &v);
			void setServName(std::vector<std::string> &v);
			void setErrorPage(std::vector<std::string> &v);
			void setLocation(TokenLine& tokenLine);
			
};
 

#endif