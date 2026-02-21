#ifndef SERVER_CONF_HPP
#define SERVER_CONF_HPP


#include <memory>
#include <map>
#include <unordered_map>
#include <utility>
#include <netdb.h>
#include <arpa/inet.h>
#include "Location.hpp"



class ServerConf
{
	private:
		uint32_t							listen_ip;
		uint16_t							listen_port;		
		std::pair<std::string,int>			listen;
		size_t								client_max_body_size;
		std::string 						root;
		std::vector<std::string>			server_name;
		std::map<int, std::string>			erro_pages;
		std::unordered_map<std::string, Location> 	locations;
		//implemente index 

		


		public:
			ServerConf();
			ServerConf& operator=(const ServerConf& other);
			~ServerConf(){};
			
			void setListen(std::string &host, int port);

			void setRoot(std::string& root);
			void conversion_listen();
			void setClientBodySize(size_t bodySize);
			void setServName(std::vector<std::string>& server_name);
			void setErrorPage(int error_code, std::string& error_path);
			void setLocation(Location& loc);

			std::string getRoot() const;
			size_t getClientSize() const;
			const int& getPort() const;
			uint32_t getIpv4() const;
			const std::vector<std::string>& getServName() const;
			const std::map<int, std::string>& getErrorPage() const;
			const std::unordered_map<std::string, Location>& getLocation() const;

			void print() const;
			
};

bool conversion_ipv4(std::string& ip, uint32_t& ipv4);

//standard port 80
//standard IP 0.0.0.0
//replace port and host to listen 
/////listen 80;                    # Porta em todas as interfaces
/////listen 8080;                  # Porta customizada
/////listen 127.0.0.1:80;         # IP específico + porta
/////listen [::]:80;              # IPv6
/////listen 80 default_server;    # Servidor padrão

#endif