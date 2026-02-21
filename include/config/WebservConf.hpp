/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservConf.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flima <flima@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 20:54:24 by flima             #+#    #+#             */
/*   Updated: 2026/02/21 18:46:35 by flima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_CONF_HPP
#define WEBSERV_CONF_HPP


#include "ServerConf.hpp"

struct ENDPOINT
{
	uint32_t	ip;
	int	port;
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
		std::map<ENDPOINT,std::vector<ServerConf>,ENDPOINTCOMP> server_pool;

		public:
			WebservConf(){};
			~WebservConf(){};
			
			// std::shared_ptr<ServerConf> findServer(std::string& host, int port);
			void 	pushServer(const ServerConf& serv, const ENDPOINT& endPoint);
			int		getNumberOfServers() const;

			void print() const;
};


//TODO
// implement a matching location match(std::string) -> return location name + string

#endif