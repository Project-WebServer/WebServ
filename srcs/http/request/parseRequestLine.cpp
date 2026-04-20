/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseRequestLine.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yulpark <yulpark@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 16:54:22 by yulpark           #+#    #+#             */
/*   Updated: 2026/04/20 11:44:54 by yulpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/http/request.hpp"

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
	return (ProtocolV::ERR);
}

feedReturn HTTPrequests::parseRequest(std::string request)
{
	size_t space1 = request.find(" ");
	size_t space2 = request.find(" ", space1 + 1);

	if (space1 == std::string::npos || space2 == std::string::npos)
		return feedReturn::NO_HOST_ERROR;
	std::string first = request.substr(0, space1);
	_methods = findMethods(first);
	//printMethod();
	if (_methods == METHODS::ERR)
		return feedReturn::METHOD_ERROR;//error code for not found issue

	_path = request.substr(space1 + 1, space2 - space1 - 1);
	if (_path.empty())
		return feedReturn::NO_HOST_ERROR;
	if (_path.find("http://") == 0) //if full address, either extract or reject
		return feedReturn::NO_HOST_ERROR;
	std::string version = request.substr(space2 + 1, request.length() - 2);

	_protocolv = findVersion(version);
	if (_protocolv == ProtocolV::ERR)
		return feedReturn::UNSUPPORTED_HTTP;
	return feedReturn::COMPLETE;
}
