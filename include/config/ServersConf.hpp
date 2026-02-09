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
		std::pair<std::string,int>	listen;
		size_t								client_max_body_size;
		std::map<std::string, Location> 	locations;


		public:
			ServerConf();
			~ServerConf();

			const std::string& get_serverName();
			const std::string& get_errorPage(std::string error_code);
			
			void setListen(TokenLine &v);
			void setClientSize(TokenLine &v);
			void setServName(TokenLine &v);
			void setErrorPage(TokenLine &v);
			void setLocation(TokenLine& tokenLine);
			
};

//starnd port 80
//standard IP 0.0.0.0
//replace port and host to listen 
/////listen 80;                    # Porta em todas as interfaces
/////listen 8080;                  # Porta customizada
/////listen 127.0.0.1:80;         # IP específico + porta
/////listen [::]:80;              # IPv6
/////listen 80 default_server;    # Servidor padrão

#endif