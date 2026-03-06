/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yulpark <yulpark@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 16:28:26 by yulpark           #+#    #+#             */
/*   Updated: 2026/03/06 16:46:18 by yulpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

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