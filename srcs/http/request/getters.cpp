/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypark <ypark@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 16:28:26 by yulpark           #+#    #+#             */
/*   Updated: 2026/03/24 15:10:32 by ypark            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/http/request.hpp"

HTTPrequests::METHODS HTTPrequests::getMethods() const
{
	return (_methods);
}

std::string HTTPrequests::getPath() const
{
	return (_path);
}

ProtocolV HTTPrequests::getProtocol() const
{
	return (_protocolv);
}

Headers	HTTPrequests::getHeader() const
{
	return (_header);
}

std::string HTTPrequests::getBody() const
{
	return (_body);
}

size_t	HTTPrequests::getContLen() const
{
	return (_contLen);
}

std::string	HTTPrequests::getContType() const
{
	return (_contType);
}

size_t HTTPrequests::getStatusCode() const
{
	return(_statusCode);
}

int		HTTPrequests::getServerPort() const
{
	return (_serverPort);
}

u_int32_t HTTPrequests::getServerIP() const
{
	return (_serverIP);
}

u_int32_t HTTPrequests::getClientIP() const
{
	return  (_clientIP);
}
