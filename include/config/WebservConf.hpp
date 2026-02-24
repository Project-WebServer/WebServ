/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservConf.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flima <flima@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 20:54:24 by flima             #+#    #+#             */
/*   Updated: 2026/02/24 20:07:39 by flima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_CONF_HPP
#define WEBSERV_CONF_HPP


#include "ServerConf.hpp"
#include "conf_parse.hpp"

struct ENDPOINT
{
	uint32_t	ip;
	int			port;
};

struct ENDPOINTCOMP
{
	bool	operator()(const ENDPOINT& a,const ENDPOINT& b) const
	{
		if (a.port != b.port)
			return a.port < b.port;
		return a.ip < b.ip;
	}
};


//store the servers 
class WebservConf
{
	private:
		std::map<ENDPOINT,std::vector<ServerConf>,ENDPOINTCOMP> virtual_servers;
		std::vector<int>										available_ports;

		public:
			WebservConf(){};
			~WebservConf(){};
			
			const std::vector<ServerConf>* matchServer(const uint32_t ipv4,const int port);
			void 	pushServer(const ServerConf& serv, const ENDPOINT& endPoint);
			int		getNumberOfServers() const;
			const std::vector<int>& getAvailablePorts();  

			void print() const;
};


//TODO
// implement a matching location match(std::string) -> return location name + string

#endif