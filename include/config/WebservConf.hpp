/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservConf.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flima <flima@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 20:54:24 by flima             #+#    #+#             */
/*   Updated: 2026/02/06 12:22:39 by flima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_CONF_HPP
#define WEBSERV_CONF_HPP


#include "ServersConf.hpp"


//store the servers 
class WebservConf
{
	private:
		std::vector<std::shared_ptr<ServersConf>> servers;

		public:
			WebservConf();
			~WebservConf();
			
			std::shared_ptr<ServersConf> findServer(std::string& host, int port);
			//setters
};
 

#endif