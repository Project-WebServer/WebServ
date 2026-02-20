/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseRequestLine.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yulpark <yulpark@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 16:54:22 by yulpark           #+#    #+#             */
/*   Updated: 2026/02/20 16:36:48 by yulpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

HTTPrequests::METHODS HTTPrequests::findMethods(std::string first)
{
	if (first == "GET")
		_methods = METHODS::GET;
	else if (first == "POST")
		_methods = METHODS::POST;
	else if (first == "DELETE")
		_methods = METHODS::DELETE;
	else
		_methods = METHODS::ERR;
	return (_methods);
}

ProtocolV HTTPrequests::findVersion(std::string version)
{
	if (version == "HTTP/1.0")
		return (ProtocolV::HTTP_1_0);
	if (version == "HTTP/1.1")
		return (ProtocolV::HTTP_1_1);
	if (version == "HTTP/2.0")
		return (ProtocolV::HTTP_2_0);
	return (ProtocolV::ERR);
}

void HTTPrequests::parseRequest(std::string request)
{
	size_t space1 = request.find(" ");
	size_t space2 = request.find(" ", space1 + 1);

	if (space1 == std::string::npos || space2 == std::string::npos)
		return ; //error code for wrong

	std::string first = request.substr(0, space1);
	_methods = findMethods(first);
	//printMethod();
	if (_methods == METHODS::ERR)
		return ;//error code for not found issue

	_path = request.substr(space1 + 1, space2 - space1 - 1);
	// what if the path is invalid? handle here or later

	std::string version = request.substr(space2 + 1, request.length() - 2);
	_protocolv = findVersion(version);
	//printVersion();
	// in case protocol v not found -> ?
}
