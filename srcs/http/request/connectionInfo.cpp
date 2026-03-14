/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connectionInfo.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypark <ypark@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 16:26:38 by yulpark           #+#    #+#             */
/*   Updated: 2026/03/14 20:31:08 by ypark            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/http/request.hpp"

void HTTPrequests::setConectionInfo(uint32_t ServerIP, uint32_t ClientIP, int Port)
{
	_serverIP = ServerIP;
	_clientIP = ClientIP;
	_serverPort = Port;
}
