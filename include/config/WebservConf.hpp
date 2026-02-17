/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservConf.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flima <flima@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 20:54:24 by flima             #+#    #+#             */
/*   Updated: 2026/02/17 11:58:39 by flima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_CONF_HPP
#define WEBSERV_CONF_HPP


#include "ServersConf.hpp"


//store the servers 
class WebservConf
{
	private:
		std::vector<ServerConf> servers;

		public:
			WebservConf(){};
			~WebservConf(){};
			
			// std::shared_ptr<ServerConf> findServer(std::string& host, int port);
			void	pushServer(ServerConf& serv);

			void print() const;
};
 
//TODO
// implement a matching location match(std::string) -> return location name + string

#endif