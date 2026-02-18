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
		size_t								client_max_body_size;
		std::string 						root; // implement root 
		std::vector<std::string>			server_name;
		std::pair<std::string,int>			listen;
		std::map<int, std::string>			erro_pages;
		std::map<std::string, Location> 	locations;

		


		public:
			ServerConf();
			ServerConf& operator=(const ServerConf& other);
			~ServerConf(){};
			
			void setListen(TokenLine &tokenLine);
			void setRoot(TokenLine &tokenLine);
			void setClientSize(TokenLine &tokenLine);
			void setServName(TokenLine &tokenLine);
			void setErrorPage(TokenLine &tokenLine);
			void setLocation(TokenLine &tokenLine, ConfToken& confile);

			std::string getRoot() const;
			size_t getClientSize() const;
			const std::pair<std::string,int>& getListen() const;
			const std::vector<std::string>& getServName() const;
			const std::map<int, std::string>& getErrorPage() const;
			const std::map<std::string, Location>& getLocation() const;

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