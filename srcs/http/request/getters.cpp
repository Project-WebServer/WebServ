/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yulpark <yulpark@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 16:28:26 by yulpark           #+#    #+#             */
/*   Updated: 2026/04/17 12:53:08 by yulpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/http/request.hpp"

HTTPrequests::METHODS HTTPrequests::getMethods() const
{
	return (_methods);
}

std::string HTTPrequests::getMethodStr()
{
	if (_methods == METHODS::GET)
		return ("GET");
	else if (_methods == METHODS::POST)
		return ("POST");
	else if (_methods == METHODS::DELETE)
		return ("DELETE");
	else
		return ("ERR");
}

std::string HTTPrequests::getVersionStr()
{
	if (_protocolv == ProtocolV::HTTP_1_0)
		return ("HTTP/1.0");
	else if (_protocolv == ProtocolV::HTTP_1_1)
		return ("HTTP/1.1");
	else if (_protocolv == ProtocolV::HTTP_2_0)
		return ("HTTP/2.0");
	else
		return ("ERR");
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

u_int32_t HTTPrequests::getClientIP() const
{
	return  (_clientIP);
}
