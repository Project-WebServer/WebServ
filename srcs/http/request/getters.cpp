/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yulpark <yulpark@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 16:28:26 by yulpark           #+#    #+#             */
/*   Updated: 2026/03/13 15:55:08 by yulpark          ###   ########.fr       */
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

size_t HTTPrequests::getStatusCode() const
{
	return(_statusCode);
}

int		HTTPrequests::getServerPort() const
{
	return (_serverPort);
}

std::string HTTPrequests::getServerIP() const
{
	return (_serverIP);
}

std::string HTTPrequests::getClientIP() const
{
	return  (_clientIP);
}
