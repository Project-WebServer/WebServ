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
		std::pair<std::string,int>			listen;
		std::vector<std::string>			server_name;
		size_t								client_max_body_size;
		std::map<int, std::string>			erro_pages;
		std::map<std::string, Location> 	locations;
		// implement root 


		public:
			ServerConf();
			ServerConf& operator=(const ServerConf& other);
			~ServerConf(){};

			const std::string& get_serverName();
			const std::string& get_errorPage(std::string error_code);
			
			void setListen(TokenLine &tokenLine); //ok
			void setClientSize(TokenLine &tokenLine); //ok
			void setServName(TokenLine &tokenLine); //ok
			void setErrorPage(TokenLine &tokenLine); //ok
			void setLocation(TokenLine &tokenLine, ConfToken& confile); //ok 

			const std::pair<std::string,int>& getListen() const; //
			size_t getClientSize() const; //
			const std::vector<std::string>& getServName() const; //
			const std::map<int, std::string>& getErrorPage(); //
			const std::map<std::string, Location>& getLocation() const; //

			void print() const;
			
};

//standard port 80
//standard IP 0.0.0.0
//replace port and host to listen 
/////listen 80;                    # Porta em todas as interfaces
/////listen 8080;                  # Porta customizada
/////listen 127.0.0.1:80;         # IP específico + porta
/////listen [::]:80;              # IPv6
/////listen 80 default_server;    # Servidor padrão

#endif