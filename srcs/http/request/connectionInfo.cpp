/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connectionInfo.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yulpark <yulpark@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 16:26:38 by yulpark           #+#    #+#             */
/*   Updated: 2026/03/13 16:29:08 by yulpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/http/request.hpp"

void HTTPrequests::setConectionInfo(std::string ServerIP, std::string ClientIP, int Port)
{
	_serverIP = ServerIP;
	_clientIP = ClientIP;
	_serverPort = Port;
}
